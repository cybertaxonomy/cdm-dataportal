/**
 * $Id: README.txt,v 1.4.2.1 2007/12/14 20:16:15 rconstantine Exp $
 * @package CCK_Address
 * @category NeighborForge
 */
This module adds an amalgamated address field to nodes via the CCK mechanisms.

There are many standard features, several of which you will never use but which were needed by the author.

--CONTENTS--
  REQUIREMENTS
  INSTALL
  FEATURES
  USAGE (Example)
  THEMING
  UNINSTALL
  CREDITS
  HELP

--REQUIREMENTS--
  This module depends on Drupal 5.X, CCK, and for some advanced features, the Activeselect module. Activeselect uses
  AJAX, but this module degrades nicely for those who haven't enabled javascript in their browser.

--INSTALL--
The site admin must first install the module per the regular drupal installation pattern. This
will:
  1) Create a table to store country names and abbreviations.
  2) Create a table to store state/province names, their abbreviations, and the country they belong to.
  3) Add U.S. data to both tables.
  4) If you install the Canada module, it will install data to both tables.
  
--FEATURES--
  8 fields make up an address in this module: Address, Address continued, Apt/suite number, City, State, ZIP, Country,
  and Other.
  
  All fields can be turned on or off. This is for both input and display.
  
  All fields can be renamed. Ex. Instead of State, you could have Province, or State/Province.
  
  The creator of the content type can order the fields as he/she wishes. I often put 'Other' first and call it 'Location
  name'. Warning, the CSS file and the themes have been designed for a certain order of fields, so you may need to
  override them.
  
  You can specify the length of these fields: Address, Address continued, Apt/suite number, City, and Other. State and
  Country are not adjustable as they are select fields.
  
  All fields can have a default value specified. State and Country are selected view select drop down (either AJAX or
  not depending on if you have Activeselect enabled), while the rest of the fields are text fields.
  
  You can reset settings to default values by deleting any changes you've made, saving the settings, then saving them
  again. [You'll see before the second save that the fields have been populated with the default values.] This does
  not apply to field order, or turning fields on/off.
  
--USAGE--
  This can be found when once you add this field to a content type via the normal CCK routine and will later be found
  here: admin/content/types/CONTENTTYPEHERE/fields/field_YOURFIELDNAMEHERE
  
  There is quite a lot here. First, this module is based around database-stored address data. What that means is that
  you can install modules that are specific to the countries you need. For example, this package comes with support for
  both the United States and Canada. The main module installs all database information for the U.S. and its states.
  Likewise, the Canadian module adds its own information to the tables created by the main module. This data, together
  with a new hook, hook_validate_address_fields, allows each module to validate the address information it is
  responsible for. See the Canada module for an example.

  --In this admin interface, you can select how the states are presented - abbreviated, full name, or free text.
    Abbreviated gives you a two letter representation. Full name is the state name spelled out in full. Free text means
    that the user can write whatever they want, rather than select from a select list as the other two methods do. You
    can also choose to abbreviate the country or not.
  
  --Next, you select which countries that this use of the field should use under 'Allowed Countries'. You can now choose
    to allow other countries. This will change the country field into a text box, rather than a selector. This bypasses
    both the database-stored countries and the database-related validation. This is required if you use 'Free-text Entry'
    for your states.
  
  --'Customize fields to use' is next and this is where you decide which address fields to display. As noted there, this
    affects both input and display. What is not mentioned there is that this does not affect storage. Blank fields are
    stored if not used.
  
  --'Maximum field lengths' are max lengths for the standard text entry fields. If you elect to make the country and state
    fields text entry as well, then they will use the max length from the city field.
    
  --'Customize field names' is where you change the names of the fields as presented to the user at the time of node
    creation or editing.
    
  --'Customize field display order' is where you order the fields from 1 to 8, which is how they will be presented. I
    suggest that you come up with just a couple of standards for your sites and stick to them as you will probably have
    to modify your theme's style.css file to override the standard css file that comes with this module as well as the
    theme functions if you do anything too radical.
    
  --'Customize field defaults' is where you can specify what fills the fields when a new node is going to be created.
    Of course, users may change the values, but for sites that are highly localized, specifying at least a City, State
    and Country (and maybe even ZIP) speeds up the entry process for such sites.

--MARGINAL USE CASE-- (ignore this if you don't need it and don't complain if it doesn't meet your needs)
  The next section in the admin is made for the rare case that addresses are known in advance. If it isn't obvious, this
  was needed for a special case by the author. Part of user registration at his site is to validate against a known address
  list stored in his database. In case you may find a similar need down the road, I'll explain how to use it.
  
  Before I begin, you need to know that the ActiveSelect module is REQUIRED for the operation of this part.
  
  The first item is a checkbox which activates the use of the rest of the items in the other parts of the module. Setting the
  other items without checking the box will do nothing.
  
  Next is a listing of every table in your database. I implemented a special permission just for this called 'administer
  databases'. This entire fieldset shouldn't even show up unless you have this permission. Select the table that contains
  your stored addresses. The next four fields should then populate with the column names from that table. In turn, select
  the column that relates to each field. Next is a country selector. Choose the country of the known addresses, then the
  state once its field is populated. Make sure the Country is selected in 'Allowed Countries' as well.
  
  That's it. The user will not be able to choose a country or a state. They will be able to select a city and a zip, though
  in my case there will always(?) only be one choice. The street number and street names are text fields for the user so that
  they don't see the possible addresses. They must already know one (hopefully their own) that is in the database as each
  field is validated alone and then together.
  
  This section of the module completely bypasses the regular validation and only validates against this table. Oh, except for
  state and country. Those use the regular tables for validation.
  
--THEMING--
  This module comes with a CSS file and implements two theme functions. I have constructed the CSS file so that two cases of
  display order should look okay when nodes are created/edited. The first case is the for the order listed in the 'Customize
  fields to use' section. The second case is where 'Other' is the first field, then 'Address', 'Apt/suite number', 'Adress
  continued', 'City', 'State', 'ZIP', and 'Country'. Also, these assume max field lengths as mentioned in that section's
  fieldset description, namely 30, 30, 7, 30, 30.
  
  Use Firefox's Firebug plugin to inspect and test changes to the CSS values, then cut and paste them into your theme's
  style.css file.
  
  If you need to override the theme functions, just copy them to the appropriate places in your theme files. I think they
  would go in node.tpl.php for standard Drupal themes.
  
  If you look at the source code, you'll find a couple of TODOs. One would be to elaborate on the node display theme. Right
  now, I have it so that the display of the fields in nodes comes out just fine in both cases I mentioned above. However,
  using complicated 'if' statements, others could probably be supported. The difficulty lies in accounting for spaces between
  fields on the same line, where to break lines, put commas, and so on and having a flexible system.
  
--UNINSTALL--
  This is done the usual way, first by deactivating, then uninstalling. A reversal of any changes made during installation is
  performed. It is best if you completely uninstall supporting modules, like Canada, and then remove the main module.
  
--CREDIT--
  This module was created by Ryan Constantine (rconstantine)
  
--HELP--
  Post issues to the issue queue of this project.