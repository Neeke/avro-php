<?php
/**
 * @author neeke@php.net
 * Date: 14-11-30 下午5:44
 */
$avro         = json_decode(file_get_contents('avro-protocol.json'), true);

$avroProtocol = new AvroProtocol();
$protocol     = $avroProtocol->real_parse($avro);
$schema       = AvroSchema::real_parse($avro['types'][0]);

$datum_writer = new AvroIODatumWriter($schema);
$write_io     = new AvroStringIO();
$encoder      = new AvroIOBinaryEncoder($write_io);

$message = array('url' => 'http://dmyz.org', 'charset' => 'utf-8');
$datum_writer->write($message, $encoder);

$content      = $write_io->string();

$headers = array(
    "POST / HTTP/1.1",
    "Host: " . self::$host,
    "Content-Type: avro/binary",
    "Content-Length: " . strlen($content),
);

$socket = stream_socket_client('localhost:9090', $errno, $errstr, 5);

if (!$socket)
    throw new Exception($errstr, $errno);

fwrite($socket, implode("\r\n", $headers) . "\r\n\r\n");
fwrite($socket, $content);

$result = '';
while (!feof($socket)) {
    $result .= fgets($socket);
}
fclose($socket);

return $result; #得到Python端返回的数据