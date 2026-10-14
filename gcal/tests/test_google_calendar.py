import json
import os
import sys
import unittest
import urllib.parse

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))

from google_calendar import Event, GoogleCalendar, events_for_day, parse_events  # noqa: E402

HERE = os.path.dirname(__file__)
SECRETS = {"client_id": "id", "client_secret": "secret", "refresh_token": "refresh"}


def sample():
    with open(os.path.join(HERE, "sample_events.json")) as f:
        return json.load(f)


class ParseTests(unittest.TestCase):
    def test_sorted_with_all_day_first(self):
        events = events_for_day(parse_events(sample()), "2026-10-14")
        self.assertEqual([e.title for e in events],
                         ["Bank holiday", "Standup (recurring instance)", "Lunch", "(no title)"])

    def test_times_are_local_clock_times(self):
        lunch = [e for e in parse_events(sample()) if e.title == "Lunch"][0]
        self.assertEqual(lunch, Event("Lunch", "2026-10-14", "12:30", "13:30"))

    def test_all_day_has_no_times(self):
        holiday = [e for e in parse_events(sample()) if e.title == "Bank holiday"][0]
        self.assertTrue(holiday.all_day)
        self.assertIsNone(holiday.start)

    def test_other_days_are_filtered_out(self):
        self.assertEqual([e.title for e in events_for_day(parse_events(sample()), "2026-10-15")], ["Dentist"])

    def test_empty_response(self):
        self.assertEqual(parse_events({}), [])


class ClientTests(unittest.TestCase):
    def make(self, clock):
        self.posts, self.gets = [], []

        def post(url, data):
            self.posts.append((url, data))
            return {"access_token": "token%d" % len(self.posts), "expires_in": 3600}

        def get(url, headers):
            self.gets.append((url, headers))
            return sample()

        return GoogleCalendar(SECRETS, post=post, get=get, now=lambda: clock[0])

    def test_refreshes_token_with_refresh_grant(self):
        cal = self.make([0])
        cal.fetch("2026-10-13T00:00:00Z", "2026-10-16T00:00:00Z")
        url, data = self.posts[0]
        self.assertEqual(data["grant_type"], "refresh_token")
        self.assertEqual(data["refresh_token"], "refresh")
        self.assertEqual(self.gets[0][1]["Authorization"], "Bearer token1")

    def test_token_reused_until_near_expiry(self):
        clock = [0]
        cal = self.make(clock)
        cal.fetch("a", "b")
        clock[0] = 3000
        cal.fetch("a", "b")
        self.assertEqual(len(self.posts), 1)
        clock[0] = 3541          # within 60s of the 3600s expiry
        cal.fetch("a", "b")
        self.assertEqual(len(self.posts), 2)
        self.assertEqual(self.gets[-1][1]["Authorization"], "Bearer token2")

    def test_request_asks_google_to_expand_recurring_events(self):
        cal = self.make([0])
        cal.fetch("2026-10-13T00:00:00Z", "2026-10-16T00:00:00Z")
        query = dict(urllib.parse.parse_qsl(urllib.parse.urlparse(self.gets[0][0]).query))
        self.assertEqual(query["singleEvents"], "true")
        self.assertEqual(query["orderBy"], "startTime")
        self.assertEqual(query["timeZone"], "Europe/London")
        self.assertIn("/calendars/primary/events", self.gets[0][0])


if __name__ == "__main__":
    unittest.main()
