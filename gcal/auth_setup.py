"""
One-off login. Run this on your laptop, NOT on the display device:

    python3 auth_setup.py path/to/client_secret.json

It opens a browser, you approve read-only calendar access, and it writes
secrets.json (gitignored) containing the refresh token. Copy that file to the
device. Standard library only.
"""

import base64
import hashlib
import http.server
import json
import os
import secrets as rand
import sys
import urllib.parse
import urllib.request
import webbrowser

AUTH_URL = "https://accounts.google.com/o/oauth2/v2/auth"
TOKEN_URL = "https://oauth2.googleapis.com/token"
SCOPE = "https://www.googleapis.com/auth/calendar.readonly"
OUTPUT = os.path.join(os.path.dirname(os.path.abspath(__file__)), "secrets.json")


def load_client(path):
    with open(path) as f:
        data = json.load(f)
    client = data.get("installed") or data.get("web")
    if not client:
        sys.exit("That does not look like a Google OAuth client file (expected a Desktop app client).")
    return client["client_id"], client["client_secret"]


class _Handler(http.server.BaseHTTPRequestHandler):
    result = {}

    def do_GET(self):
        params = dict(urllib.parse.parse_qsl(urllib.parse.urlparse(self.path).query))
        _Handler.result = params
        self.send_response(200)
        self.send_header("Content-Type", "text/plain; charset=utf-8")
        self.end_headers()
        self.wfile.write(b"Done. You can close this tab and go back to the terminal.")

    def log_message(self, *args):
        pass


def main():
    if len(sys.argv) != 2:
        sys.exit(__doc__)

    client_id, client_secret = load_client(sys.argv[1])

    server = http.server.HTTPServer(("127.0.0.1", 0), _Handler)
    redirect = "http://127.0.0.1:%d" % server.server_port

    # PKCE protects the code exchange; state protects against forged redirects
    verifier = base64.urlsafe_b64encode(os.urandom(48)).rstrip(b"=").decode()
    challenge = base64.urlsafe_b64encode(hashlib.sha256(verifier.encode()).digest()).rstrip(b"=").decode()
    state = rand.token_urlsafe(16)

    url = AUTH_URL + "?" + urllib.parse.urlencode({
        "client_id": client_id,
        "redirect_uri": redirect,
        "response_type": "code",
        "scope": SCOPE,
        "access_type": "offline",   # needed to receive a refresh token
        "prompt": "consent",        # forces a refresh token even on repeat logins
        "state": state,
        "code_challenge": challenge,
        "code_challenge_method": "S256",
    })

    print("Opening your browser. If nothing opens, visit:\n\n" + url + "\n")
    webbrowser.open(url)
    server.handle_request()

    params = _Handler.result
    if params.get("state") != state or "code" not in params:
        sys.exit("Login failed: %s" % params.get("error", "unexpected response"))

    body = urllib.parse.urlencode({
        "client_id": client_id,
        "client_secret": client_secret,
        "code": params["code"],
        "code_verifier": verifier,
        "grant_type": "authorization_code",
        "redirect_uri": redirect,
    }).encode()
    with urllib.request.urlopen(urllib.request.Request(TOKEN_URL, data=body)) as r:
        tokens = json.loads(r.read())

    if "refresh_token" not in tokens:
        sys.exit("Google did not return a refresh token. Revoke the app at "
                 "https://myaccount.google.com/permissions and run this again.")

    with open(OUTPUT, "w") as f:
        json.dump({
            "client_id": client_id,
            "client_secret": client_secret,
            "refresh_token": tokens["refresh_token"],
            "calendar_id": "primary",
            "time_zone": "Europe/London",
        }, f, indent=2)
    os.chmod(OUTPUT, 0o600)
    print("Saved %s. Copy it to the display device and keep it out of git." % OUTPUT)


if __name__ == "__main__":
    main()
