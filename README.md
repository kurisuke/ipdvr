ipdvr - A digital video recorder for IPTV streams
=================================================

Planned Features
----------------

* daemon-like operation mode
* interaction over socket by json protocol (schedule recordings, get
  listings, etc.)
* record of RTP streams using ffmpeg
* background fetching of TV listings (json.xmltv.se)
* scheduling of single and series recordings
* library management of recordings

- Build system: CMake
- Listings DB: sqlite
- All things JSON: nlohmann/json

Status
------

This project is in planning / design stage. See doc/outline.txt for a
detailed list of requirements / proposed building blocks.

Attributions
------------

ipdvr includes the following 3rd-party libraries:

- json/json.hpp -- JSON parser
    - https://github.com/nlohmann/json
    - license: COPYING.json
