# Example watch control file for uscan
# Rename this file to "watch" and then you can run the "uscan" command
# to check for upstream updates and more.
# Site		Directory		Pattern			Version	Script
version=2
sunsite.unc.edu	/pub/Linux/Incoming	openser-1.0.0-(.*)\.tar\.gz	debian	uupdate
