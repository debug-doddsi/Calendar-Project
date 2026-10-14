"""Print today's events from your real calendar. Needs secrets.json (see auth_setup.py)."""

import datetime
import json
import os

from google_calendar import GoogleCalendar, events_for_day

HERE = os.path.dirname(os.path.abspath(__file__))


def main():
    with open(os.path.join(HERE, "secrets.json")) as f:
        calendar = GoogleCalendar(json.load(f))

    today = datetime.date.today()
    # Fetch a day either side so timezone edges cannot hide events; filter by local date after
    fmt = "%Y-%m-%dT00:00:00Z"
    events = calendar.fetch((today - datetime.timedelta(days=1)).strftime(fmt),
                            (today + datetime.timedelta(days=2)).strftime(fmt))

    for e in events_for_day(events, today.isoformat()):
        print("%s  %s" % ("all day" if e.all_day else e.start, e.title))


if __name__ == "__main__":
    main()
