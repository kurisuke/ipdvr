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

- contrib/json/json.hpp -- JSON parser
    - https://github.com/nlohmann/json
    - license: COPYING.json
- contrib/cxxopts/cxxopts.hpp -- JSON parser
    - https://github.com/jarro2783/cxxopts
    - license: COPYING.cxxopts
