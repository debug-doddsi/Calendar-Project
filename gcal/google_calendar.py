"""
Google Calendar client for the LED calendar.

Standard library only, so it runs on a laptop and is easy to port to MicroPython.
All network access goes through the `post` and `get` functions passed to
GoogleCalendar, so tests can run with no network.
"""

import json
import urllib.parse
import urllib.request

TOKEN_URL = "https://oauth2.googleapis.com/token"
EVENTS_URL = "https://www.googleapis.com/calendar/v3/calendars/{calendar}/events"

# Keep the response small: only what the display needs
EVENT_FIELDS = "items(summary,start,end)"


class Event:
    """One calendar entry. `date` is YYYY-MM-DD, `start`/`end` are HH:MM or None for all day."""

    def __init__(self, title, date, start, end):
        self.title = title
        self.date = date
        self.start = start
        self.end = end

    @property
    def all_day(self):
        return self.start is None

    def __eq__(self, other):
        return isinstance(other, Event) and vars(self) == vars(other)

    def __repr__(self):
        return "Event(%r, %r, %r, %r)" % (self.title, self.date, self.start, self.end)


def parse_events(response):
    """Turn an events.list response into a sorted list of Events.

    Times are read straight from the string. The API is asked for the local
    timeZone, so the clock time in the string is already local time. Slicing
    avoids needing timezone support, which MicroPython lacks.
    """
    events = []
    for item in response.get("items", []):
        title = item.get("summary", "(no title)")
        start = item.get("start", {})
        end = item.get("end", {})

        if "dateTime" in start:
            stamp = start["dateTime"]            # 2026-10-14T09:00:00+01:00
            end_stamp = end.get("dateTime", stamp)
            events.append(Event(title, stamp[:10], stamp[11:16], end_stamp[11:16]))
        elif "date" in start:
            events.append(Event(title, start["date"], None, None))

    # All-day events first, then by start time
    events.sort(key=lambda e: (e.date, e.start or ""))
    return events


def events_for_day(events, date):
    """Filter a list of Events to one YYYY-MM-DD date."""
    return [e for e in events if e.date == date]


def _http_post(url, data):
    body = urllib.parse.urlencode(data).encode()
    with urllib.request.urlopen(urllib.request.Request(url, data=body)) as r:
        return json.loads(r.read())


def _http_get(url, headers):
    with urllib.request.urlopen(urllib.request.Request(url, headers=headers)) as r:
        return json.loads(r.read())


class GoogleCalendar:
    def __init__(self, secrets, post=_http_post, get=_http_get, now=None):
        """`secrets` needs client_id, client_secret, refresh_token and optionally
        calendar_id (default "primary") and time_zone (default "Europe/London")."""
        self._client_id = secrets["client_id"]
        self._client_secret = secrets["client_secret"]
        self._refresh_token = secrets["refresh_token"]
        self._calendar = secrets.get("calendar_id", "primary")
        self._time_zone = secrets.get("time_zone", "Europe/London")
        self._post = post
        self._get = get
        self._token = None
        self._token_expiry = 0
        self._now = now  # seconds since epoch, injectable for tests

    def _time(self):
        import time
        return self._now() if self._now else time.time()

    def access_token(self):
        """Return a valid access token, refreshing it if it expires within a minute."""
        if self._token is None or self._time() >= self._token_expiry - 60:
            reply = self._post(TOKEN_URL, {
                "client_id": self._client_id,
                "client_secret": self._client_secret,
                "refresh_token": self._refresh_token,
                "grant_type": "refresh_token",
            })
            self._token = reply["access_token"]
            self._token_expiry = self._time() + int(reply.get("expires_in", 3600))
        return self._token

    def fetch(self, time_min, time_max):
        """Fetch events between two RFC3339 timestamps (e.g. 2026-10-13T00:00:00Z)."""
        query = urllib.parse.urlencode({
            "singleEvents": "true",   # Google expands recurring events for us
            "orderBy": "startTime",
            "timeMin": time_min,
            "timeMax": time_max,
            "timeZone": self._time_zone,
            "fields": EVENT_FIELDS,
        })
        url = EVENTS_URL.format(calendar=urllib.parse.quote(self._calendar, safe="")) + "?" + query
        reply = self._get(url, {"Authorization": "Bearer " + self.access_token()})
        return parse_events(reply)
