This is a modified version of cck_fullname.module

This module adds a full name CCK field to your installation. There are two sets of five fields,
prefix, first, middle, last, and suffix. There are several configuration options allowing you
to use legal names and preferred names.

--CONTENTS--
  REQUIREMENTS
  INSTALL/SETUP
  FEATURES
  USAGE (Example)
  FUTURE
  UNINSTALL
  CREDITS
  MIGRATION
  HELP
  
--REQUIREMENTS--
  This module requires the CCK content module.
  
  This module also supports the DIFF module and the TOKENS module. If you find something wrong
  with the support for these modules, please post and issue on the project's issue queue,
  preferably with a solution.
  
--INSTALL/SETUP--
  Download and ungzip/untar the package and place it wherever you have you other modules installed.
  
  If this is a new installation, that's it because this only uses CCK database stuff and CCK takes
  care of most of it.
  
  If this is an update to an existing install of this module, you'll need to run the Drupal update
  script. I added an install file because CCK doesn't take care of changes to the tables it
  manages. Hopefully someone has/will fix this in a future release of CCK.
  
--FEATURES--
  Two sets of five field names; 'legal name' and 'preferred name'.
  
  'Legal' is the default name with options of REQUIRED and OPTIONAL while 'preferred' has options
  of REQUIRED, OPTIONAL and HIDDEN.
  
  The five fields each name has are prefix, first, middle, last, and suffix. You may turn any of
  these fields on or off. You may REQUIRE individual fields without REQUIRING the entire name.
  This has the effect of only coming into effect if ANY field for that name is filled in.
  
  You may specify the maximum length of every field.
  
  You may limit middle names to initials.
  
  Multiple values works. If you have a name that is REQUIRED, only the first pair of names is
  affected as would be expected with multiple entries.
  
--USAGE--
  ADMINS-------
    Create a content type as you would normally. See the CCK docs for more info.
    
    Add a content type. Name it as you wish and select the 'fullname' widget. Save your choice. You
    are then presented with the settings page.
    
    Fill the upper section as you see fit, then turn your attention to the bottom section labeled
    'Data settings'. Here you will find all of the admin settings for this cck field set.
    
    At the top is a note I've added and is labeled "Notes:..." Read it and copy the note to your
    users into the 'Help text' section above it. Feel free to change the wording to reflect the 
    options you choose below.
    
    Next are two check boxes. The first enables the option to have multiple fields. If you don't
    know why you'd need this, you probably don't. The second check box is for restricting the
    middle name to 1 character, for storage as an initial. This is probably redundant since you
    could manually specify a field size of 1 below.
    
    The next section is 'Legal Name' and has several radio buttons and check boxes in it. The radio
    button set is for choosing the overall REQUIREMENT of the 'Legal name'. The 'Legal name' is the
    default for this module so there are only two options for use - REQUIRED and OPTIONAL. I'll
    explain the interaction of all options after I lay them all out. Keep in mind that this is a
    setting for the WHOLE NAME. The check boxes allow you to show/hide fields for input. If you
    don't care about prefixes and suffixes, for example, just uncheck them. Default is unchecked.
    
    The next section is 'Preferred Name' and is the same as the section just outlined except for
    one addition. There is an extra radio button labeled DON'T USE. This will hide this second
    name field from your users and effectively makes this a single name module.
    
    The next section is 'Required Parts' and lists all of ten parts of both name fields. Checking
    these boxes makes them individually REQUIRED. They only go into effect when the checkboxes
    in the previous two sections are also checked. In other words, REQUIRING hidden fields has no
    effect and no side effects. So feel free to check them all if you like.
    
    The above three sections interact in this way:
      Scenario A - You have marked one of the name's radio buttons as REQUIRED and checked all
      boxes in both its section and the 'Required Parts' section. This will behave as any other
      REQUIRED field you are use to where all sub-fields are also required.
      
      Scenario B - You have marked one of the name's radio buttons as REQUIRED and checked all
      boxes in its section, but only some in the 'Required Parts' section. The user must fill
      in only those fields marked in the 'Required Parts' section and can leave the others blank.
      
      Scenario C - You have marked one of the name's radio buttons as OPTIONAL and checked all
      boxes in both its section and the 'Required Parts' section. This means that the user won't
      have to fill in any fields, but if they choose to fill in one, they must fill them all in.
      
      Scenario D - You have marked one of the name's radio buttons as OPTIONAL and checked all
      boxes in its section, but only some in the 'Required Parts' section. The user must fill
      in only those fields marked in the 'Required Parts' section, and only if they fill in ANY
      of the fields for that name.
      
      Scenario E - I don't think there is one.
      
      NOTE: For display purposes, only one name is displayed on a node. If the user has specified
            a preferred name, that is used. If not, then the legal name is used. Use the translation
            features of Drupal to change any terms you don't like.
            
    The next two sections are identical, but for each of the two names in turn. In them, you may
    specify the maximum lengths of all fields.
  
  USERS-------
    From a user's perspective, there really isn't much to see. At node creation/editing, a table
    is presented to the user. The header is filled with the field names of which ever name has more
    fields. The legal name is on the first row, followed by the preferred name on the second. If
    you have multiple fields enabled, these two will alternate. Only the first two are affected by
    NAME-WIDE REQUIREMENTS. Latter names are only affected by FIELD REQUIREMENTS. In other words,
    all names after the first pair operate as though they have the OPTIONAL setting, even if one
    or both have the REQUIRED setting for the first rows.
    
    If you set the 'Preferred Name' to DON'T SHOW, then those rows won't even show up in the table.
    If you choose not to use individual fields, they won't show for that name type. If both name
    types don't have that field, then it is eliminated from the table.
    
    Validation works for any number of names in any REQUIREMENT combination.
    
    A side note: if you use the messageFX module, required fields flash if missing when submitted.
    That's pretty cool.
    
  THEMES-------
    I have mad an effort to sprinkle DIV and SPAN throughout. This release adds a CSS file which
    is pretty much necessary to get the REQUIRED indicator (*) to show and be in the right place
    since this complex interaction of REQUIREMENTS can't use Drupal's defaults. Feel free to
    override both the CSS entries and the theme functions.
    
--FUTURE--
  I would like to get this VIEWS compatible, but I currently don't have time to figure it out.
  Patches are very welcome. Also, if you'd like more output/display options, let me know, or
  roll patches for that as well. I've got a few options, but haven't actually figured out how to
  use them myself. I think someone contributed them earlier and I have simply updated them.
  
--UNINSTALL--
  Just disable the module. CCK should do its thing to remove any remnants (unless they still
  haven't fixed those issues).
  
--CREDITS--
  This module developed by Ryan Constantine (rconstantine). Thanks to linuxbox for ideas and a
  little bit of code (in the admin settings) for the new features. That was about all I could
  integrate of his code into mine for features that his module had that this didn't. I think I
  have now integrated all features from his module into this one. If you see his module's page
  (namefield module) you'll see that he didn't know this module pre-dated his and he agreed he'd
  rather there just be one. Hopefully, this module will work for him.
  
--MIGRATION--
  If you are currently using his (linuxbox) module (namefield) and would like to move to this
  one, encourage him to write an update routine. With the same number of fields for the names,
  it could possibly be as simple as renaming database columns. Admin settings are resaved each
  time a field is edited and saved, so if you don't have too many such fields, you could update
  the admin settings by hand easily enough. And if you only have one or two field instances, you
  could probably even update the database fields (and references) by hand too.
  
--HELP--
  As usual, go to http://drupal.org/project/issues/expertsdb_fullname to post issues of support, bug
  reports, and feature requests. Contribute to the code if you can. I have too many modules to
  keep on top of everything all of the time.