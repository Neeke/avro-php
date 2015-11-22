#!/usr/bin/env python
# -*- coding:utf-8 -*-
from BaseHTTPServer import BaseHTTPRequestHandler, HTTPServer
try:
    from cStringIO import StringIO
except ImportError:
    from StringIO import StringIO

import cgi
import json
from avro import io, ipc, protocol, schema

PROTOCOL_FILE = 'avro-protocol.json'
PROTOCOL_JSON = json.loads(open(PROTOCOL_FILE).read())
PROTOCOL = protocol.parse(open(PROTOCOL_FILE).read())
SCHEMA = schema.parse(json.dumps(PROTOCOL_JSON['types'][0]))

SERVER_ADDR = ('localhost', 9090)

class PostRequestHandler(BaseHTTPRequestHandler):
    def do_POST(self):

        length       = int(self.headers.getheader('content-length'))
        post_data    = self.rfile.read(length)

        data_reader  = StringIO(post_data)
        decoder      = io.BinaryDecoder(data_reader)

        datum_reader = io.DatumReader(SCHEMA)
        data         = datum_reader.read(decoder)
        url          = data['url']
        charset      = data['charset']
        #已经取得从PHP发过来的数据，在这里执行其他逻辑
        self.wfile.write('title') #返回取到的title

if __name__ == '__main__':
    server = HTTPServer(SERVER_ADDR, PostRequestHandler)
    server.allow_reuse_address = True
    server.serve_forever()