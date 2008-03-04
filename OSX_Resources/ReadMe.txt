

$Id: README 15541M 2008-03-03 21:44:10Z (local) $

This is the OPeNDAP cdf Data Handler. It is used along with the OPeNDAPDAP Server3 and Server4 1.2 beta.

For information about building the OPeNDAP cdf Data Handler, see theINSTALL file.

This data handler is a component of the OPeNDAP DAP Server; the server basesoftware is designed to allow any number of handlers to be configured easily.See the DAP Server README and INSTALL files for information aboutconfiguration, including how to use this handler.

A configuration edition helper script, `bes-cdf-data.sh' is provided in this package for easy configuration of the Hyrax BES server, designed to editbes.conf. The script is called using:

   bes-cdf-data.sh [<bes.conf file to modify> [<bes modules dir>]]


The `bes-conf' make target runs the script while trying to select paths cleverly, and should be called using:

   make bes-conf


Test data are also installed, so after installing this handler, Hyrax will have data to serve providing an easy way to test your new installationand to see how a working bes.conf should look. To use this, make surethat you first install the bes, and that dap-server gets installed too.Finally, every time you install or reinstall handlers, make sure to restartthe BES and OLFS.

