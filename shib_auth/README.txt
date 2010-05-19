$Id: README.txt,v 1.1.4.2 2008/07/16 11:54:03 niif Exp $
***********************************************************
Shibboleth Authetication module - Installation Instructions

Writen by Tamás Dévai: tamas.devai@niif.hu

The Module for Drupal5 seems to be discontinued by niif.hu 
therefore we now host it in the EDIT svn repositry. 
Andreas Kohlbecker 2009-09-17
***********************************************************

Module description:
You can use Shibboleth 1.3 or above authentication whit this module in Drupal
system.

Configure:
The Shibboleth handler's URL: This is the shib SP handler url. By default is
/Shibboleth.sso, but you can change that in apache config. REQUIRE

The Shibboleth handler's protocol: http or https. Https is very recommended in
production site! REQUIRE

The WAYF's location: this is url of the Where Are You From service. OPTIONAL

Generated log-in link from the settings: 
  - whit WAYF
    https://example.com/Shibboleth.sso/WAYF/INSTITUTE-WAYF?target=https://example.com/

  - whitout WAYF
    https://example.com/Shibboleth.sso/?target=https://example.com/
