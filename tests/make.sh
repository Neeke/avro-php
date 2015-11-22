
cp -rf test_php_ext/* test_php_ext.make/

cd test_php_ext.make

phpize

./configure

make

make install

cd ../

#/etc/init.d/apache2 restart
