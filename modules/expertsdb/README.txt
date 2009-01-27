$Id$

REQUIREMENTS
==========================================================

Drupal Modules:
----------------------------------------------------------
  activeselect (activeselect-5.x-1.0.tar.gz)
  admin_menu  
  auto_nodetitle (auto_nodetitle-5.x-1.0.tar.gz)
  captcha
  captcha_pack (captcha_pack-5.x-1.1.tar.gz)
  cck (cck-5.x-1.6-1.tar.gz with cck_nodereference_views_fusion_47.patch)
  cck_field_perms (cck_field_perms-5.x-1.10.tar.gz)
  cck_taxonomy (cck_taxonomy-5.x-1.2.tar.gz)
  cck_taxonomy_ssu
  contemplate
  content_access
  date (date-5.x-1.6.tar.gz)
  devel (devel-5.x-1.x-dev.tar.gz) - not essentially required, however useful for administration.
  invisimail (invisimail-5.x-1.0.tar.gz)
  jquery_plugin (jquery_plugin-5.x-1.3.tar.gz)
  jquery_update
  jstools
  location
  nodefamily (nodefamily-5.x-1.3.tar.gz)
  nodeprofile (nodeprofile-5.x-1.2.tar.gz)
  roleassign
  sitedoc - not essentially required, however useful for administration.
  smtp
  subform_element (subform_element-5.x-1.4.tar.gz)
  taxonomy_menu
  taxonomy_xml
  textfield_autocomplete
  tinymce
  token (token-5.x-1.9.tar.gz)
  usernode (usernode-5.x-1.4.tar.gz)
  views (views-5.x-1.6.tar.gz)
  views_fusion (views_fusion-5.x-1.2.tar.gz)
  

Some Drupal Modules which need to be patched 

  see ./module-patches for patches

Additional patches found not shipped with the expertsdb module:

	- Patch for cck_field_perms.module v 1.3.2.27 : 
	  	http://dev.e-taxonomy.eu/svn/trunk/Drupal/module_patches/cck_field_perms/cck_field_perms-views0.1.patch
	  	
	- Patch for nodefamily.module v 1.19.2.14
		http://dev.e-taxonomy.eu/svn/trunk/Drupal/module_patches/nodefamily/nodefamily_create-others-nodes.patch
    
    
THEMES
----------------------------------------------------------
EDIT-expertsdb (http://dev.e-taxonomy.eu/svn/trunk/Drupal/themes/EDIT-expertsdb)

INSTALL
==========================================================

If you have downloaded the complete installation package, installation requires only a few steps. (If you have checked out the expertsdb module from out svn repository you need to download, patch and install the drupal modules which are listed above.)

Installation of install package:

1. extract the archive to the document root of your webserver.
2. configure the mysql database in the settings.php file 
3. import the mysql dump
4. navigate to admin/build/themes and to admin/build/modules in order to refresh the paths stored in the database

READY!


Importing Taxonomy resources
==========================================================
1.) Import all taxonomies xml files from ./expertDB/resources/ 

When importing the taxonomies by the taxonomy_xml it is highly recommended that you set 
the max_execution_time and max_input_time in the php.ini to a value which is high  
enough to prevent from interrupting the import process. 
We use the following setting. Five Minutes of max_execution_time should be sufficient:

	max_execution_time = 300     
	max_input_time = 120
	

IMPORTANT: You must check the 'Allow duplicate terms' option when importing 
	- DAISIE taxonomy structure.import.xml
	- geography-ed2.import.xml 
	
Once all taxonomies are imported you should restore the former values of max_execution_time and max_input_time.  
