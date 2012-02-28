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
		)
		,
		'distribution' => array(
			'view_name'=> 'dwca_export_distribution',
			'display_id' => 'views_data_export_1',
			'out_file_url' => NULL
		)
		,
		'description' => array(
							'general' => array(
								'view_name'=> 'dwca_export_description_general',
								'display_id' => 'views_data_export_1',
								'out_file_url' => NULL
			),
							'morphology' => array(
								'view_name'=> 'dwca_export_description_morph',
								'display_id' => 'views_data_export_1',
								'out_file_url' => NULL
			),
							'behaviour' => array(
								'view_name'=> 'dwca_export_description_behav',
								'display_id' => 'views_data_export_1',
								'out_file_url' => NULL
			),
							'biology' => array(
								'view_name'=> 'dwca_export_description_biol',
								'display_id' => 'views_data_export_1',
								'out_file_url' => NULL
			),
							'conservation_status' => array(
								'view_name'=> 'dwca_export_description_cons',
								'display_id' => 'views_data_export_1',
								'out_file_url' => NULL
			),
							'legislation' => array(
								'view_name'=> 'dwca_export_description_leg',
								'display_id' => 'views_data_export_1',
								'out_file_url' => NULL
			),
							'management' => array(
								'view_name'=> 'dwca_export_description_manage',
								'display_id' => 'views_data_export_1',
								'out_file_url' => NULL
			),
							'procedures' => array(
								'view_name'=> 'dwca_export_description_proc',
								'display_id' => 'views_data_export_1',
								'out_file_url' => NULL
			),
							'threats' => array(
								'view_name'=> 'dwca_export_description_threat',
								'display_id' => 'views_data_export_1',
								'out_file_url' => NULL
			),
							'associations' => array(
								'view_name'=> 'dwca_export_description_assoc',
								'display_id' => 'views_data_export_1',
								'out_file_url' => NULL
			),
							'cyclicity' => array(
								'view_name'=> 'dwca_export_description_cyclic',
								'display_id' => 'views_data_export_1',
								'out_file_url' => NULL
			),
							'diagnostic_description' => array(
								'view_name'=> 'dwca_export_description_diag',
								'display_id' => 'views_data_export_1',
								'out_file_url' => NULL
			),
							'diseases' => array(
								'view_name'=> 'dwca_export_description_disease',
								'display_id' => 'views_data_export_1',
								'out_file_url' => NULL
			),
							'dispersal' => array(
								'view_name'=> 'dwca_export_description_disperse',
								'display_id' => 'views_data_export_1',
								'out_file_url' => NULL
			),
							'threats' => array(
								'view_name'=> 'dwca_export_description_morph',
								'display_id' => 'views_data_export_1',
								'out_file_url' => NULL
			),
							'distribution' => array(
								'view_name'=> 'dwca_export_description_dist',
								'display_id' => 'views_data_export_1',
								'out_file_url' => NULL
			),
							'ecology' => array(
								'view_name'=> 'dwca_export_description_ecol',
								'display_id' => 'views_data_export_1',
								'out_file_url' => NULL
			),
							'evolution' => array(
								'view_name'=> 'dwca_export_description_evolve',
								'display_id' => 'views_data_export_1',
								'out_file_url' => NULL
			),
							'genetics' => array(
								'view_name'=> 'dwca_export_description_genet',
								'display_id' => 'views_data_export_1',
								'out_file_url' => NULL
			),
							'growth' => array(
								'view_name'=> 'dwca_export_description_grow',
								'display_id' => 'views_data_export_1',
								'out_file_url' => NULL
			),
							'habitat' => array(
								'view_name'=> 'dwca_export_description_habit',
								'display_id' => 'views_data_export_1',
								'out_file_url' => NULL
			),
							'life_cycle' => array(
								'view_name'=> 'dwca_export_description_life',
								'display_id' => 'views_data_export_1',
								'out_file_url' => NULL
			),
							'life_expectancy' => array(
								'view_name'=> 'dwca_export_description_life_ex',
								'display_id' => 'views_data_export_1',
								'out_file_url' => NULL
			),
							'look_alikes' => array(
								'view_name'=> 'dwca_export_description_look',
								'display_id' => 'views_data_export_1',
								'out_file_url' => NULL
			),
							'migration' => array(
								'view_name'=> 'dwca_export_description_migr',
								'display_id' => 'views_data_export_1',
								'out_file_url' => NULL
			),
							'molecular_biology' => array(
								'view_name'=> 'dwca_export_description_molec',
								'display_id' => 'views_data_export_1',
								'out_file_url' => NULL
			),
							'physiology' => array(
								'view_name'=> 'dwca_export_description_phys',
								'display_id' => 'views_data_export_1',
								'out_file_url' => NULL
			),
							'population_biology' => array(
								'view_name'=> 'dwca_export_description_popul',
								'display_id' => 'views_data_export_1',
								'out_file_url' => NULL
			),
							'reproduction' => array(
								'view_name'=> 'dwca_export_description_reprod',
								'display_id' => 'views_data_export_1',
								'out_file_url' => NULL
			),
							'risk_statement' => array(
								'view_name'=> 'dwca_export_description_risk',
								'display_id' => 'views_data_export_1',
								'out_file_url' => NULL
			),
							'size' => array(
								'view_name'=> 'dwca_export_description_size',
								'display_id' => 'views_data_export_1',
								'out_file_url' => NULL
			),
							'taxon_biology' => array(
								'view_name'=> 'dwca_export_description_taxbio',
								'display_id' => 'views_data_export_1',
								'out_file_url' => NULL
			),
							'trophic_strategy' => array(
								'view_name'=> 'dwca_export_description_troph',
								'display_id' => 'views_data_export_1',
								'out_file_url' => NULL
			),
							'uses' => array(
								'view_name'=> 'dwca_export_description_uses',
								'display_id' => 'views_data_export_1',
								'out_file_url' => NULL
			)

		)
	);
	$export['dwca_export_archive_descriptor_file_map'] = $strongarm;
	
	//throw new Exception(t('Error creating the archive'));

	return $export;
}