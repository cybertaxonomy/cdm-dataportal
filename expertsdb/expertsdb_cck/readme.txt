***********
* README: *
***********

DESCRIPTION:
------------
This is a set of CCK field types supporting the experts database EDIT (European Distributed Institute of Taxonomy)

Some of these module provide functionality for multiple valies as a set of serialized data in one db field.
To keep these fields in the content types db table (e.g. to denormalize the data structure),
*DO NOT REUSE EXISTING CONTENT FIELDS DURING CONTENT TYPE CREATION*, but create new fiels with new names,
*OTHERWISE* Drupal will automatically normalize the dataset and create external db tables for that field.

Creating separate db tables for data fields set to multiple values is suppressed by the modules themselves.


experstdb_address.module provides an address field type for cck
	Multiple data will be integrated as serialized value into the according content type database.

expertsdb_email.module provides an email field type for CCK.
	Multiple data will be integrated as serialized value into the according content type database.

expertsdb_fullname.module
	Multiple values are deactivated.

expertsdb_alias.module provides a name alias field type for CCK.
	Multiple data will be integrated as serialized value into the according content type database.

expertsdb_phone.module provides a phone number field type for CCK.
	Multiple data will be integrated as serialized value into the according content type database.

expertsdb_link.module provides a link field type for CCK with optional title.
	Multiple data will be integrated as serialized value into the according content type database.

expertsdb_timespan.module provides two date fields representing a lifespan.
	Multiple values are deactivated.

expertsdb_addnode.module provides the same funtionality like addnode.module
	Misc modifications regarding form rendering (inline styles removed, ...)

REQUIREMENTS:
-------------
  - all modules require the CCK content module.

  - the expertsdb_fullname.module supports the diff.module and the tokens.module.

	- the expertsdb_address.module supports activeselect.module, which is highly recomemnded

  - The expertsdb_email.module requires jquery_plugin.module with checkboxes-plugin to be installed.
	  For optional encryption of email addresses: invisimail.module


INSTALLATION:
-------------
1. Place the entire expertsdb_cck directory into your Drupal modules/
   directory.

2. Enable the needed expertsdb_cck modules by navigating to:

     administer > modules


Features:
---------
expertsdb_email.module
	* Validation of emails
	* Rurns addresses into a mailto link
	* Encryption of addresses to avoid spam (requires invisimail.module)
	* Contact form to hide mail addresses completely
	* Writes preferred email address to user account, if set.
	* Checks and deletes duplicate email addresses
	* Checks, if newly set preferred email address is already in use with another user account, if set.
	* Takes an unlimited number of email addresses and writes them as a single serialized value to the content types db table.
	* Default value is always the users account email address, if set
	* User has to check exactly one email address as the preferred email address.
	* Adding additional email addresses is 'AHAH' powered for a fluent user experience.
	* Utilizes jQuery plugin 'checkboxes' to turn chackboxes into radioCheckboxes (as far as radioButtons couldn't be used).
	* Email form is CAPTCHA-enabled
	* If node-PrivacyLevel is set to ContactPrivate, only the primary email address is turned into a email form link
  * Access to email forms is configurable via access control


expertsdb_fullname.module
	* Minor tweaks so far


expertsdb_alias.module
	* Formatters to display Aliases as default values, as comma separated values or as comma separated values in round brackets
	* Takes an unlimited number of name aliases and writes them as a single serialized value to the content types db table.
	* Adding additional aliases is 'AJAX' powered for a fluent user experience


expertsdb_phone.module
	* Takes an unlimited number of name aliases and writes them as a single serialized value to the content types db table.
	* Adding additional aliases is 'AJAX' powered for a fluent user experience
	* checks for basic international phone number notation
	* Has optional title fields for all phone numbers. Can be set to required


expertsdb_link.module
	* Validation of URLs
	* Turn URL into a linked URL
	* Misc formatters to display URLs as plain URL, Title with URL link, Title prefixed to the linked URL
	* Checks for correct url protocol
	* Takes an unlimited number of urls with optional title and writes them as a single serialized value to the content types db table.
	* Adding additional urls is 'AHAH' powered for a fluent user experience


expertsdb_timespan.module
	* Timespan From field may be required, Timespan To field is always optional
	* Takes dates in either format YYYY/MM/DD, YYYY/MM or YYYY
	* Delimiter is /, nothing else.
	* Field labels are editable
	* Provides a default formatter displaying:
		still allive: [Label timespan_from] YYYY/MM/DD
		already died: YYYY/MM/DD - YYYY/MM/DD
	* Provides a christian formatter (WARNING: this formatting migth be inappropriate for non-christian users!)
		still alive: * YYYY/MM/DD
		already died: * YYYY/MM/DD, † YYYY/MM/DD
	* Provides a membership formatter
		still member: Member since
		not member anymore: Member from YYYY/MM/DD to YYYY/MM/DD


expertsdb_addnode.module
	ATTENTION: addnode.module itself uses tables with inline styles to format the select and create-new forms.
	These tables have been remove to get more space for big forms.
	1. addnode.module produces tons of invalid HTML-code due to multiple id-reusage
	2. the module supposedly does not work correctly without javascript
	3. the module has useability issues: when deciding to create a new node and choosing again to pick a
	present node, formerly selected items have been deselected. They should  be reselected.
	4. addnode subforms are submitted without setting the node author leading to nodes created by anonymous
	und thus being unable to track nodes created by a specific user.
	@TODO: these problems should bee fixed or at least tested for further problems


expertsdb_addres.module
	* Provides a list of all countries, but not for all regions
	* Provides adding of multiple address fields (AHAH-powered)
	* Provides a flag to set one address as the prefreed address
	* Provides a list of all countries (see http://en.wikipedia.org/wiki/ISO_3166-1_alpha-2)


Kudos:
------
expertsdb_email.module is based on email.module by
	Matthias Hutterer
	mh86@drupal.org
	m_hutterer@hotmail.com

expertsdb_link.module is base on link.module by
	Nathan Haug aka quicksketch
	http://drupal.org/user/35821

expertsdb_addnode.module is based on addnode.module by
	Obslogic (Mike Smith aka Lionfish)

expertsdb_address.module and its extensions are based on cck_address.module and cck_address_extensions.module by
	Ryan Constantine (rconstantine)

Author:
-------
Nils Clark-Bernhard
nils.clark-bernhard@human-aspects.de
http://www.human-aspects.de
