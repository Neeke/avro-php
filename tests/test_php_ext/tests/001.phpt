--TEST_AVRO--
Check for test_avro presence
--SKIPIF--
<?php if (!extension_loaded("test_avro")) print "skip"; ?>
--FILE--
<?php 
echo "test_avro extension is available";
/*
	you can add regression test_avros for your extension here

  the output of your test_avro code has to be equal to the
  text in the --EXPECT-- section below for the test_avros
  to pass, differences between the output and the
  expected text are interpreted as failure

	see php5/README.TEST_AVROING for further information on
  writing regression test_avros
*/
?>
--EXPECT--
test_avro extension is available
