
REQUIREMENTS
------------------------------
Drupal Modules:

	- auto_nodetitle
	- cck (with cck_nodereference_views_fusion_47.patch)
	- cck_taxonomy_ssu
	- subform_element
	- taxonomy_xml
	- views
	- views_fusion


Drupal Modules which need to be patched:

	- Patch for cck_field_perms.module v 1.3.2.27 : 
	  	http://dev.e-taxonomy.eu/svn/trunk/Drupal/module_patches/cck_field_perms/cck_field_perms-views0.1.patch
	  	
	- Patch for nodefamily.module v 1.19.2.14
		http://dev.e-taxonomy.eu/svn/trunk/Drupal/module_patches/nodefamily/nodefamily_create-others-nodes.patch

INSTALL
------------------------------

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
