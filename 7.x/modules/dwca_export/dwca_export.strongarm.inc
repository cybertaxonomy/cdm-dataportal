<?php

/**
 * Set up the map containing the file name to view name mapping with default values
 * @return multitype:stdClass
 */
function dwca_export_strongarm(){

	$export = array();

	$strongarm = new stdClass;
	$strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
	$strongarm->api_version = 1;
	$strongarm->name = 'dwca_export_archive_descriptor_file_map';
	$strongarm->value = array(
		'classification' => array(
			'view_name'=> 'dwca_export_classification',
			'display_id' => 'views_data_export_1',
			'out_file_url' => NULL
		),
		'typesandspecimen' => array(
			'view_name'=> 'dwca_export_specimen',
			'display_id' => 'views_data_export_1',
			'out_file_url' => NULL
		),
		'reference' => array(
			'view_name'=> 'dwca_export_reference',
			'display_id' => 'views_data_export_1',
			'out_file_url' => NULL
		),
		'image' => array(
			'view_name'=> 'dwca_export_image',
			'display_id' => 'views_data_export_1',
			'out_file_url' => NULL
		),
		//'description' => array(
			//'view_name'=> 'dwca_export_description',
			//'display_id' => 'views_data_export_1',
			//'out_file_url' => NULL
		//)
	'description' => array(
				'view_name'=> 'dwca_export_description',
				'display_id' => 'views_data_export_1',
				'out_file_url' => NULL
	)
	);
	$export['dwca_export_archive_descriptor_file_map'] = $strongarm;
	
	//throw new Exception(t('Error creating the archive'));

	return $export;
}