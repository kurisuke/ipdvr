ipdvr - A digital video recorder for IPTV streams
=================================================

Planned Features
----------------

* daemon-like operation mode
* integrated HTTP server with RESTful api (schedule recordings,
  get listings, etc.)
* record of RTP streams using ffmpeg
* background fetching of TV listings (json.xmltv.se)
* scheduling of single and series recordings
* library management of recordings

Build requirements
------------------

* C++11 supporting compiler (tested with GCC, Clang)
* libcurl
* SQLite3

Status
------

This project is in planning / design stage. See doc/outline.txt for a
detailed list of requirements / proposed building blocks.

Attributions
------------

ipdvr includes the following 3rd-party code:

- contrib/json -- JSON parser
    - https://github.com/nlohmann/json
- contrib/cxxopts -- Command-line option parser
    - https://github.com/jarro2783/cxxopts
- contrib/sqlite3pp -- SQLite wrapper
    - https://github.com/iwongu/sqlite3pp
