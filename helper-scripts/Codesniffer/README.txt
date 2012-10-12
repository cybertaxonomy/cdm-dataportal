Drupal_CDM is a sniff for Drupal CDM Portals. It can be used with PHP CodeSniffer
to check the syntax of the PHP code in the CDM_Dataportal modules and themes. It
requires the Drupal sniff to be present.

This sniff is based on the Drupal sniff but has the check for camel case
variables disabled, since DRUPAL CDM Portals use variable names that should be
the same as in the CDM Server Java implementation.