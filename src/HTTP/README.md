# HTTP

## How to Use This
    HTTPRequest req("url");
    HTTPRequest.addURI("username", "abc");
    HTTPRequest.addCookie("username", "cookie");
    HTTPRequest.sendRequest("outfile");//Or nullptr if you don't care about the result

## Implementation
This is just a thin wrapper around libCURL so will work on any platform libCURL works on.
