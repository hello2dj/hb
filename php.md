Installing PHP
The only thing to consider in this section is to remove any previous PHP versions on
your system. To do so, you can run the following command:

$ sudo apt-get -y purge php.*

The next step is to add the necessary repositories in order to fetch the correct PHP
version. The commands to add and update them are:

$ sudo apt-get install python-software-properties
$ sudo LC_ALL=en_US.UTF-8 add-apt-repository ppa:ondrej/php -y
$ sudo apt-get update

Finally, we need to install PHP 7 together with the driver for MySQL. For this,
just execute the following three commands:

$ sudo apt-get install php7.0 php7.0-fpm php7.0-mysql -y
$ sudo apt-get --purge autoremove -y
$ sudo service php7.0-fpm start