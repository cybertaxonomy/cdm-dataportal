<?php

/**
 * Implementation of hook_views_default_views().
 *
 * @see dwca_export_views_api()
 */
function dwca_export_views_default_views() {
	static $view_functions = array('classification', 'specimen', 'reference', 'image', 'distribution', 'description', 
	'description_general', 'description_morphology', 'description_behaviour', 'description_biology',
	'description_conservation_status', 'description_legislation', 'description_management', 'description_procedures',
	'description_threats','description_associations','description_cyclicity','description_diagnostic_description',
	'description_diseases', 'description_dispersal', 'description_distribution', 'description_ecology',
	'description_evolution', 'description_genetics' ,'description_growth', 'description_habitat', 
	'description_life_cycle', 'description_life_expectancy', 'description_look_alikes', 'description_migration',
	'description_molecular_biology', 'description_physiology', 'description_population_biology', 'description_reproduction',
	'description_risk_statement', 'description_size', 'description_taxon_biology', 'description_trophic_strategy', 
	'description_uses');

	$views = array();

	foreach ($view_functions as $function_postfix) {
		$function = '_view_' . $function_postfix;
		$view = $function();
		$views[$view->name] = $view;
		watchdog("dwca_export", "hook_views_default_views() : view '".$view->name."'");
	}
	return $views;
}

/**
 * Classification Export View - exported from views module
 */
function _view_classification() {

	$view = new view;
	$view->name = 'dwca_export_classification';
	$view->description = 'DwC-A core data for a Taxon';
	$view->tag = 'default';
	$view->base_table = 'taxonomy_term_data';
	$view->human_name = 'DwC-A Export Classification';
	$view->core = 7;
	$view->api_version = '3.0';
	$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
	
	/* Display: Master */
	$handler = $view->new_display('default', 'Master', 'default');
	$handler->display->display_options['title'] = 'Classification Export View';
	$handler->display->display_options['access']['type'] = 'perm';
	$handler->display->display_options['cache']['type'] = 'none';
	$handler->display->display_options['query']['type'] = 'views_query';
	$handler->display->display_options['query']['options']['query_comment'] = FALSE;
	$handler->display->display_options['exposed_form']['type'] = 'basic';
	$handler->display->display_options['pager']['type'] = 'none';
	$handler->display->display_options['pager']['options']['offset'] = '0';
	$handler->display->display_options['style_plugin'] = 'table';
	/* Relationship: relParent */
	$handler->display->display_options['relationships']['parent']['id'] = 'parent';
	$handler->display->display_options['relationships']['parent']['table'] = 'taxonomy_term_hierarchy';
	$handler->display->display_options['relationships']['parent']['field'] = 'parent';
	$handler->display->display_options['relationships']['parent']['ui_name'] = 'relParent';
	$handler->display->display_options['relationships']['parent']['required'] = 0;
	/* Field: Taxonomy term: Term ID */
	$handler->display->display_options['fields']['tid_2']['id'] = 'tid_2';
	$handler->display->display_options['fields']['tid_2']['table'] = 'taxonomy_term_data';
	$handler->display->display_options['fields']['tid_2']['field'] = 'tid';
	$handler->display->display_options['fields']['tid_2']['label'] = 'id';
	$handler->display->display_options['fields']['tid_2']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['tid_2']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['tid_2']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['tid_2']['alter']['external'] = 0;
	$handler->display->display_options['fields']['tid_2']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['tid_2']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['tid_2']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['tid_2']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['tid_2']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['tid_2']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['tid_2']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['tid_2']['alter']['html'] = 0;
	$handler->display->display_options['fields']['tid_2']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['tid_2']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['tid_2']['hide_empty'] = 0;
	$handler->display->display_options['fields']['tid_2']['empty_zero'] = 0;
	$handler->display->display_options['fields']['tid_2']['hide_alter_empty'] = 0;
	$handler->display->display_options['fields']['tid_2']['separator'] = '';
	$handler->display->display_options['fields']['tid_2']['format_plural'] = 0;
	/* Field: Taxonomy term: Name */
	$handler->display->display_options['fields']['name_1']['id'] = 'name_1';
	$handler->display->display_options['fields']['name_1']['table'] = 'taxonomy_term_data';
	$handler->display->display_options['fields']['name_1']['field'] = 'name';
	$handler->display->display_options['fields']['name_1']['label'] = 'scientificName';
	$handler->display->display_options['fields']['name_1']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['name_1']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['name_1']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['name_1']['alter']['external'] = 0;
	$handler->display->display_options['fields']['name_1']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['name_1']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['name_1']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['name_1']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['name_1']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['name_1']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['name_1']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['name_1']['alter']['html'] = 0;
	$handler->display->display_options['fields']['name_1']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['name_1']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['name_1']['hide_empty'] = 0;
	$handler->display->display_options['fields']['name_1']['empty_zero'] = 0;
	$handler->display->display_options['fields']['name_1']['hide_alter_empty'] = 0;
	$handler->display->display_options['fields']['name_1']['link_to_taxonomy'] = 0;
	/* Field: Taxonomy term: Term ID */
	$handler->display->display_options['fields']['tid_3']['id'] = 'tid_3';
	$handler->display->display_options['fields']['tid_3']['table'] = 'taxonomy_term_data';
	$handler->display->display_options['fields']['tid_3']['field'] = 'tid';
	$handler->display->display_options['fields']['tid_3']['relationship'] = 'parent';
	$handler->display->display_options['fields']['tid_3']['label'] = 'parentNameUsageId';
	$handler->display->display_options['fields']['tid_3']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['tid_3']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['tid_3']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['tid_3']['alter']['external'] = 0;
	$handler->display->display_options['fields']['tid_3']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['tid_3']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['tid_3']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['tid_3']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['tid_3']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['tid_3']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['tid_3']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['tid_3']['alter']['html'] = 0;
	$handler->display->display_options['fields']['tid_3']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['tid_3']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['tid_3']['hide_empty'] = 1;
	$handler->display->display_options['fields']['tid_3']['empty_zero'] = 0;
	$handler->display->display_options['fields']['tid_3']['hide_alter_empty'] = 0;
	$handler->display->display_options['fields']['tid_3']['separator'] = '';
	$handler->display->display_options['fields']['tid_3']['format_plural'] = 0;
	/* Field: Taxonomy term: Name */
	$handler->display->display_options['fields']['name_4']['id'] = 'name_4';
	$handler->display->display_options['fields']['name_4']['table'] = 'taxonomy_term_data';
	$handler->display->display_options['fields']['name_4']['field'] = 'name';
	$handler->display->display_options['fields']['name_4']['relationship'] = 'parent';
	$handler->display->display_options['fields']['name_4']['label'] = 'parentNameUsage';
	$handler->display->display_options['fields']['name_4']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['name_4']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['name_4']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['name_4']['alter']['external'] = 0;
	$handler->display->display_options['fields']['name_4']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['name_4']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['name_4']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['name_4']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['name_4']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['name_4']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['name_4']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['name_4']['alter']['html'] = 0;
	$handler->display->display_options['fields']['name_4']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['name_4']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['name_4']['hide_empty'] = 0;
	$handler->display->display_options['fields']['name_4']['empty_zero'] = 0;
	$handler->display->display_options['fields']['name_4']['hide_alter_empty'] = 0;
	$handler->display->display_options['fields']['name_4']['link_to_taxonomy'] = 0;
	/* Field: Taxonomy vocabulary: Name */
	$handler->display->display_options['fields']['name']['id'] = 'name';
	$handler->display->display_options['fields']['name']['table'] = 'taxonomy_vocabulary';
	$handler->display->display_options['fields']['name']['field'] = 'name';
	$handler->display->display_options['fields']['name']['label'] = 'datasetName';
	$handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['name']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['name']['alter']['external'] = 0;
	$handler->display->display_options['fields']['name']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['name']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['name']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['name']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['name']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['name']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['name']['alter']['html'] = 0;
	$handler->display->display_options['fields']['name']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['name']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['name']['hide_empty'] = 0;
	$handler->display->display_options['fields']['name']['empty_zero'] = 0;
	$handler->display->display_options['fields']['name']['hide_alter_empty'] = 0;
	/* Field: Taxonomy term: Unit name 1 */
	$handler->display->display_options['fields']['field_unit_name1']['id'] = 'field_unit_name1';
	$handler->display->display_options['fields']['field_unit_name1']['table'] = 'field_data_field_unit_name1';
	$handler->display->display_options['fields']['field_unit_name1']['field'] = 'field_unit_name1';
	$handler->display->display_options['fields']['field_unit_name1']['label'] = 'uninomial';
	$handler->display->display_options['fields']['field_unit_name1']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_unit_name1']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_unit_name1']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_unit_name1']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_unit_name1']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_unit_name1']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_unit_name1']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_unit_name1']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_unit_name1']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_unit_name1']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_unit_name1']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_unit_name1']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_unit_name1']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_unit_name1']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_unit_name1']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_unit_name1']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_unit_name1']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_unit_name1']['field_api_classes'] = 0;
	/* Field: Taxonomy term: Unit name 2 */
	$handler->display->display_options['fields']['field_unit_name2']['id'] = 'field_unit_name2';
	$handler->display->display_options['fields']['field_unit_name2']['table'] = 'field_data_field_unit_name2';
	$handler->display->display_options['fields']['field_unit_name2']['field'] = 'field_unit_name2';
	$handler->display->display_options['fields']['field_unit_name2']['label'] = 'specificEpithet';
	$handler->display->display_options['fields']['field_unit_name2']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_unit_name2']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_unit_name2']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_unit_name2']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_unit_name2']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_unit_name2']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_unit_name2']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_unit_name2']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_unit_name2']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_unit_name2']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_unit_name2']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_unit_name2']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_unit_name2']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_unit_name2']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_unit_name2']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_unit_name2']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_unit_name2']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_unit_name2']['field_api_classes'] = 0;
	/* Field: Taxonomy term: Unit name 3 */
	$handler->display->display_options['fields']['field_unit_name3']['id'] = 'field_unit_name3';
	$handler->display->display_options['fields']['field_unit_name3']['table'] = 'field_data_field_unit_name3';
	$handler->display->display_options['fields']['field_unit_name3']['field'] = 'field_unit_name3';
	$handler->display->display_options['fields']['field_unit_name3']['label'] = 'infraspecificEpithet';
	$handler->display->display_options['fields']['field_unit_name3']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_unit_name3']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_unit_name3']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_unit_name3']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_unit_name3']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_unit_name3']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_unit_name3']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_unit_name3']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_unit_name3']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_unit_name3']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_unit_name3']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_unit_name3']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_unit_name3']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_unit_name3']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_unit_name3']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_unit_name3']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_unit_name3']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_unit_name3']['field_api_classes'] = 0;
	/* Field: Taxonomy term: Usage */
	$handler->display->display_options['fields']['field_usage']['id'] = 'field_usage';
	$handler->display->display_options['fields']['field_usage']['table'] = 'field_data_field_usage';
	$handler->display->display_options['fields']['field_usage']['field'] = 'field_usage';
	$handler->display->display_options['fields']['field_usage']['label'] = 'taxonomicStatus';
	$handler->display->display_options['fields']['field_usage']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_usage']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_usage']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_usage']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_usage']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_usage']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_usage']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_usage']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_usage']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_usage']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_usage']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_usage']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_usage']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_usage']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_usage']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_usage']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_usage']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_usage']['field_api_classes'] = 0;
	/* Field: Taxonomy term: Associated accepted name */
	$handler->display->display_options['fields']['field_aan_cichorieae']['id'] = 'field_aan_cichorieae';
	$handler->display->display_options['fields']['field_aan_cichorieae']['table'] = 'field_data_field_aan_cichorieae';
	$handler->display->display_options['fields']['field_aan_cichorieae']['field'] = 'field_aan_cichorieae';
	$handler->display->display_options['fields']['field_aan_cichorieae']['label'] = 'acceptedNameUsage';
	$handler->display->display_options['fields']['field_aan_cichorieae']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_aan_cichorieae']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_aan_cichorieae']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_aan_cichorieae']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_aan_cichorieae']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_aan_cichorieae']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_aan_cichorieae']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_aan_cichorieae']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_aan_cichorieae']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_aan_cichorieae']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_aan_cichorieae']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_aan_cichorieae']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_aan_cichorieae']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_aan_cichorieae']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_aan_cichorieae']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_aan_cichorieae']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_aan_cichorieae']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_aan_cichorieae']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_aan_cichorieae']['field_api_classes'] = 0;
	/* Field: Taxonomy term: Authors */
	$handler->display->display_options['fields']['field_authors']['id'] = 'field_authors';
	$handler->display->display_options['fields']['field_authors']['table'] = 'field_data_field_authors';
	$handler->display->display_options['fields']['field_authors']['field'] = 'field_authors';
	$handler->display->display_options['fields']['field_authors']['label'] = 'scientificNameAuthorship';
	$handler->display->display_options['fields']['field_authors']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_authors']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_authors']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_authors']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_authors']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_authors']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_authors']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_authors']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_authors']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_authors']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_authors']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_authors']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_authors']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_authors']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_authors']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_authors']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_authors']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_authors']['field_api_classes'] = 0;
	/* Field: Taxonomy term: Reference */
	$handler->display->display_options['fields']['field_reference']['id'] = 'field_reference';
	$handler->display->display_options['fields']['field_reference']['table'] = 'field_data_field_reference';
	$handler->display->display_options['fields']['field_reference']['field'] = 'field_reference';
	$handler->display->display_options['fields']['field_reference']['label'] = 'bibliographicCitation';
	$handler->display->display_options['fields']['field_reference']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_reference']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_reference']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_reference']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_reference']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_reference']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_reference']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_reference']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_reference']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_reference']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_reference']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_reference']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_reference']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_reference']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_reference']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_reference']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_reference']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_reference']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_reference']['field_api_classes'] = 0;
	/* Field: Taxonomy term: Vernacular names */
	$handler->display->display_options['fields']['field_vernacular_names']['id'] = 'field_vernacular_names';
	$handler->display->display_options['fields']['field_vernacular_names']['table'] = 'field_data_field_vernacular_names';
	$handler->display->display_options['fields']['field_vernacular_names']['field'] = 'field_vernacular_names';
	$handler->display->display_options['fields']['field_vernacular_names']['label'] = 'vernacularName';
	$handler->display->display_options['fields']['field_vernacular_names']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_vernacular_names']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_vernacular_names']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_vernacular_names']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_vernacular_names']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_vernacular_names']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_vernacular_names']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_vernacular_names']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_vernacular_names']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_vernacular_names']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_vernacular_names']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_vernacular_names']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_vernacular_names']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_vernacular_names']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_vernacular_names']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_vernacular_names']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_vernacular_names']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_vernacular_names']['group_rows'] = 1;
	$handler->display->display_options['fields']['field_vernacular_names']['delta_offset'] = '0';
	$handler->display->display_options['fields']['field_vernacular_names']['delta_reversed'] = 0;
	$handler->display->display_options['fields']['field_vernacular_names']['delta_first_last'] = 0;
	$handler->display->display_options['fields']['field_vernacular_names']['field_api_classes'] = 0;
	/* Field: Taxonomy term: Rank */
	$handler->display->display_options['fields']['field_rank']['id'] = 'field_rank';
	$handler->display->display_options['fields']['field_rank']['table'] = 'field_data_field_rank';
	$handler->display->display_options['fields']['field_rank']['field'] = 'field_rank';
	$handler->display->display_options['fields']['field_rank']['label'] = 'taxonRank';
	$handler->display->display_options['fields']['field_rank']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_rank']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_rank']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_rank']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_rank']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_rank']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_rank']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_rank']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_rank']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_rank']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_rank']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_rank']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_rank']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_rank']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_rank']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_rank']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_rank']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_rank']['field_api_classes'] = 0;
	/* Sort criterion: Taxonomy term: Term ID */
	$handler->display->display_options['sorts']['tid']['id'] = 'tid';
	$handler->display->display_options['sorts']['tid']['table'] = 'taxonomy_term_data';
	$handler->display->display_options['sorts']['tid']['field'] = 'tid';
	
	/* Display: Page */
	$handler = $view->new_display('page', 'Page', 'page');
	$handler->display->display_options['path'] = 'dwca_export/classification/page';
	
	/* Display: Data export */
	$handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
	$handler->display->display_options['pager']['type'] = 'none';
	$handler->display->display_options['pager']['options']['offset'] = '0';
	$handler->display->display_options['style_plugin'] = 'views_data_export_csv';
	$handler->display->display_options['style_options']['provide_file'] = 0;
	$handler->display->display_options['style_options']['parent_sort'] = 0;
	$handler->display->display_options['style_options']['quote'] = 1;
	$handler->display->display_options['style_options']['trim'] = 0;
	$handler->display->display_options['style_options']['replace_newlines'] = 1;
	$handler->display->display_options['style_options']['newline_replacement'] = '\\r';
	$handler->display->display_options['style_options']['header'] = 1;
	$handler->display->display_options['path'] = 'dwca_export/classification/csv';
	

	return $view;
}

function _view_reference() {

	$view = new view;
	$view->name = 'dwca_export_reference';
	$view->description = 'DwC-A reference extension for a Taxon';
	$view->tag = 'default';
	$view->base_table = 'node';
	$view->human_name = 'DwC-A Export Reference';
	$view->core = 7;
	$view->api_version = '3.0';
	$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

	/* Display: Master */
	$handler = $view->new_display('default', 'Master', 'default');
	$handler->display->display_options['title'] = 'Reference Export';
	$handler->display->display_options['access']['type'] = 'perm';
	$handler->display->display_options['cache']['type'] = 'none';
	$handler->display->display_options['query']['type'] = 'views_query';
	$handler->display->display_options['query']['options']['query_comment'] = FALSE;
	$handler->display->display_options['exposed_form']['type'] = 'basic';
	$handler->display->display_options['pager']['type'] = 'full';
	$handler->display->display_options['pager']['options']['items_per_page'] = '10';
	$handler->display->display_options['style_plugin'] = 'table';
	$handler->display->display_options['style_options']['columns'] = array(
  'biblio_number' => 'biblio_number',
  'title' => 'title',
	);
	$handler->display->display_options['style_options']['default'] = '-1';
	$handler->display->display_options['style_options']['info'] = array(
  'biblio_number' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
	),
  'title' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
	),
	);
	$handler->display->display_options['style_options']['override'] = 1;
	$handler->display->display_options['style_options']['sticky'] = 0;
	$handler->display->display_options['style_options']['empty_table'] = 0;
	/* Relationship: Content: Taxonomy terms on node */
	$handler->display->display_options['relationships']['term_node_tid']['id'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['table'] = 'node';
	$handler->display->display_options['relationships']['term_node_tid']['field'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['required'] = 0;
	/*$handler->display->display_options['relationships']['term_node_tid']['vocabularies'] = array(
  'cichorieae' => 0,
  'image_keywords' => 0,
  'imaging_technique' => 0,
  'preparation_technique' => 0,
	);*/
	/* Field: Taxonomy term: Term ID */
	$handler->display->display_options['fields']['tid']['id'] = 'tid';
	$handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
	$handler->display->display_options['fields']['tid']['field'] = 'tid';
	$handler->display->display_options['fields']['tid']['relationship'] = 'term_node_tid';
	$handler->display->display_options['fields']['tid']['label'] = 'id';
	$handler->display->display_options['fields']['tid']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['external'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['html'] = 0;
	$handler->display->display_options['fields']['tid']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['tid']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['tid']['hide_empty'] = 0;
	$handler->display->display_options['fields']['tid']['empty_zero'] = 0;
	$handler->display->display_options['fields']['tid']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['tid']['format_plural'] = 0;
	/* Field: Biblio: Accession Number */
	$handler->display->display_options['fields']['biblio_accession_number']['id'] = 'biblio_accession_number';
	$handler->display->display_options['fields']['biblio_accession_number']['table'] = 'biblio';
	$handler->display->display_options['fields']['biblio_accession_number']['field'] = 'biblio_accession_number';
	$handler->display->display_options['fields']['biblio_accession_number']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['biblio_accession_number']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['biblio_accession_number']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['biblio_accession_number']['alter']['external'] = 0;
	$handler->display->display_options['fields']['biblio_accession_number']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['biblio_accession_number']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['biblio_accession_number']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['biblio_accession_number']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['biblio_accession_number']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['biblio_accession_number']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['biblio_accession_number']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['biblio_accession_number']['alter']['html'] = 0;
	$handler->display->display_options['fields']['biblio_accession_number']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['biblio_accession_number']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['biblio_accession_number']['hide_empty'] = 0;
	$handler->display->display_options['fields']['biblio_accession_number']['empty_zero'] = 0;
	$handler->display->display_options['fields']['biblio_accession_number']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['biblio_accession_number']['biblio_label'] = 1;
	/* Field: Biblio: Call Number */
	$handler->display->display_options['fields']['biblio_call_number']['id'] = 'biblio_call_number';
	$handler->display->display_options['fields']['biblio_call_number']['table'] = 'biblio';
	$handler->display->display_options['fields']['biblio_call_number']['field'] = 'biblio_call_number';
	$handler->display->display_options['fields']['biblio_call_number']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['biblio_call_number']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['biblio_call_number']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['biblio_call_number']['alter']['external'] = 0;
	$handler->display->display_options['fields']['biblio_call_number']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['biblio_call_number']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['biblio_call_number']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['biblio_call_number']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['biblio_call_number']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['biblio_call_number']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['biblio_call_number']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['biblio_call_number']['alter']['html'] = 0;
	$handler->display->display_options['fields']['biblio_call_number']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['biblio_call_number']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['biblio_call_number']['hide_empty'] = 0;
	$handler->display->display_options['fields']['biblio_call_number']['empty_zero'] = 0;
	$handler->display->display_options['fields']['biblio_call_number']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['biblio_call_number']['biblio_label'] = 1;
	/* Field: Biblio: Citation Key */
	$handler->display->display_options['fields']['biblio_citekey']['id'] = 'biblio_citekey';
	$handler->display->display_options['fields']['biblio_citekey']['table'] = 'biblio';
	$handler->display->display_options['fields']['biblio_citekey']['field'] = 'biblio_citekey';
	$handler->display->display_options['fields']['biblio_citekey']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['biblio_citekey']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['biblio_citekey']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['biblio_citekey']['alter']['external'] = 0;
	$handler->display->display_options['fields']['biblio_citekey']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['biblio_citekey']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['biblio_citekey']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['biblio_citekey']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['biblio_citekey']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['biblio_citekey']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['biblio_citekey']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['biblio_citekey']['alter']['html'] = 0;
	$handler->display->display_options['fields']['biblio_citekey']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['biblio_citekey']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['biblio_citekey']['hide_empty'] = 0;
	$handler->display->display_options['fields']['biblio_citekey']['empty_zero'] = 0;
	$handler->display->display_options['fields']['biblio_citekey']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['biblio_citekey']['biblio_label'] = 1;
	/* Field: Biblio: DOI */
	$handler->display->display_options['fields']['biblio_doi']['id'] = 'biblio_doi';
	$handler->display->display_options['fields']['biblio_doi']['table'] = 'biblio';
	$handler->display->display_options['fields']['biblio_doi']['field'] = 'biblio_doi';
	$handler->display->display_options['fields']['biblio_doi']['alter']['alter_text'] = 1;
	$handler->display->display_options['fields']['biblio_doi']['alter']['text'] = 'doi:[biblio_doi]';
	$handler->display->display_options['fields']['biblio_doi']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['biblio_doi']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['biblio_doi']['alter']['external'] = 0;
	$handler->display->display_options['fields']['biblio_doi']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['biblio_doi']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['biblio_doi']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['biblio_doi']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['biblio_doi']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['biblio_doi']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['biblio_doi']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['biblio_doi']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['biblio_doi']['alter']['html'] = 0;
	$handler->display->display_options['fields']['biblio_doi']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['biblio_doi']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['biblio_doi']['hide_empty'] = 0;
	$handler->display->display_options['fields']['biblio_doi']['empty_zero'] = 0;
	$handler->display->display_options['fields']['biblio_doi']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['biblio_doi']['biblio_label'] = 1;
	/* Field: Biblio: ISBN Number */
	$handler->display->display_options['fields']['biblio_isbn']['id'] = 'biblio_isbn';
	$handler->display->display_options['fields']['biblio_isbn']['table'] = 'biblio';
	$handler->display->display_options['fields']['biblio_isbn']['field'] = 'biblio_isbn';
	$handler->display->display_options['fields']['biblio_isbn']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['biblio_isbn']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['biblio_isbn']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['biblio_isbn']['alter']['external'] = 0;
	$handler->display->display_options['fields']['biblio_isbn']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['biblio_isbn']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['biblio_isbn']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['biblio_isbn']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['biblio_isbn']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['biblio_isbn']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['biblio_isbn']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['biblio_isbn']['alter']['html'] = 0;
	$handler->display->display_options['fields']['biblio_isbn']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['biblio_isbn']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['biblio_isbn']['hide_empty'] = 0;
	$handler->display->display_options['fields']['biblio_isbn']['empty_zero'] = 0;
	$handler->display->display_options['fields']['biblio_isbn']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['biblio_isbn']['biblio_label'] = 1;
	/* Field: Biblio: ISSN Number */
	$handler->display->display_options['fields']['biblio_issn']['id'] = 'biblio_issn';
	$handler->display->display_options['fields']['biblio_issn']['table'] = 'biblio';
	$handler->display->display_options['fields']['biblio_issn']['field'] = 'biblio_issn';
	$handler->display->display_options['fields']['biblio_issn']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['biblio_issn']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['biblio_issn']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['biblio_issn']['alter']['external'] = 0;
	$handler->display->display_options['fields']['biblio_issn']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['biblio_issn']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['biblio_issn']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['biblio_issn']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['biblio_issn']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['biblio_issn']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['biblio_issn']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['biblio_issn']['alter']['html'] = 0;
	$handler->display->display_options['fields']['biblio_issn']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['biblio_issn']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['biblio_issn']['hide_empty'] = 0;
	$handler->display->display_options['fields']['biblio_issn']['empty_zero'] = 0;
	$handler->display->display_options['fields']['biblio_issn']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['biblio_issn']['biblio_label'] = 1;
	/* Field: Biblio: Other Numbers */
	$handler->display->display_options['fields']['biblio_other_number']['id'] = 'biblio_other_number';
	$handler->display->display_options['fields']['biblio_other_number']['table'] = 'biblio';
	$handler->display->display_options['fields']['biblio_other_number']['field'] = 'biblio_other_number';
	$handler->display->display_options['fields']['biblio_other_number']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['biblio_other_number']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['biblio_other_number']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['biblio_other_number']['alter']['external'] = 0;
	$handler->display->display_options['fields']['biblio_other_number']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['biblio_other_number']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['biblio_other_number']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['biblio_other_number']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['biblio_other_number']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['biblio_other_number']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['biblio_other_number']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['biblio_other_number']['alter']['html'] = 0;
	$handler->display->display_options['fields']['biblio_other_number']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['biblio_other_number']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['biblio_other_number']['hide_empty'] = 0;
	$handler->display->display_options['fields']['biblio_other_number']['empty_zero'] = 0;
	$handler->display->display_options['fields']['biblio_other_number']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['biblio_other_number']['biblio_label'] = 1;
	/* Field: Biblio: Biblio Citation */
	$handler->display->display_options['fields']['citation']['id'] = 'citation';
	$handler->display->display_options['fields']['citation']['table'] = 'biblio';
	$handler->display->display_options['fields']['citation']['field'] = 'citation';
	$handler->display->display_options['fields']['citation']['label'] = 'bibliographicCitation';
	$handler->display->display_options['fields']['citation']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['citation']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['citation']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['citation']['alter']['external'] = 0;
	$handler->display->display_options['fields']['citation']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['citation']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['citation']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['citation']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['citation']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['citation']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['citation']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['citation']['alter']['html'] = 0;
	$handler->display->display_options['fields']['citation']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['citation']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['citation']['hide_empty'] = 0;
	$handler->display->display_options['fields']['citation']['empty_zero'] = 0;
	$handler->display->display_options['fields']['citation']['hide_alter_empty'] = 1;
	/* Field: Content: Title */
	$handler->display->display_options['fields']['title']['id'] = 'title';
	$handler->display->display_options['fields']['title']['table'] = 'node';
	$handler->display->display_options['fields']['title']['field'] = 'title';
	$handler->display->display_options['fields']['title']['label'] = 'title';
	$handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['title']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['title']['alter']['external'] = 0;
	$handler->display->display_options['fields']['title']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['title']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['title']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['title']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['title']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['title']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['title']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['title']['alter']['html'] = 0;
	$handler->display->display_options['fields']['title']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['title']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['title']['hide_empty'] = 0;
	$handler->display->display_options['fields']['title']['empty_zero'] = 0;
	$handler->display->display_options['fields']['title']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['title']['link_to_node'] = 1;
	/* Field: Biblio: Authors */
	$handler->display->display_options['fields']['biblio_authors']['id'] = 'biblio_authors';
	$handler->display->display_options['fields']['biblio_authors']['table'] = 'biblio';
	$handler->display->display_options['fields']['biblio_authors']['field'] = 'biblio_authors';
	$handler->display->display_options['fields']['biblio_authors']['label'] = 'creater';
	$handler->display->display_options['fields']['biblio_authors']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['biblio_authors']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['biblio_authors']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['biblio_authors']['alter']['external'] = 0;
	$handler->display->display_options['fields']['biblio_authors']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['biblio_authors']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['biblio_authors']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['biblio_authors']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['biblio_authors']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['biblio_authors']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['biblio_authors']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['biblio_authors']['alter']['html'] = 0;
	$handler->display->display_options['fields']['biblio_authors']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['biblio_authors']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['biblio_authors']['hide_empty'] = 0;
	$handler->display->display_options['fields']['biblio_authors']['empty_zero'] = 0;
	$handler->display->display_options['fields']['biblio_authors']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['biblio_authors']['biblio_label'] = 1;
	$handler->display->display_options['fields']['biblio_authors']['auth_category'] = '1';
	$handler->display->display_options['fields']['biblio_authors']['initialize'] = 1;
	/* Field: Biblio: Secondary Title */
	$handler->display->display_options['fields']['biblio_secondary_title']['id'] = 'biblio_secondary_title';
	$handler->display->display_options['fields']['biblio_secondary_title']['table'] = 'biblio';
	$handler->display->display_options['fields']['biblio_secondary_title']['field'] = 'biblio_secondary_title';
	$handler->display->display_options['fields']['biblio_secondary_title']['label'] = 'source';
	$handler->display->display_options['fields']['biblio_secondary_title']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['biblio_secondary_title']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['biblio_secondary_title']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['biblio_secondary_title']['alter']['external'] = 0;
	$handler->display->display_options['fields']['biblio_secondary_title']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['biblio_secondary_title']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['biblio_secondary_title']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['biblio_secondary_title']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['biblio_secondary_title']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['biblio_secondary_title']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['biblio_secondary_title']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['biblio_secondary_title']['alter']['html'] = 0;
	$handler->display->display_options['fields']['biblio_secondary_title']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['biblio_secondary_title']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['biblio_secondary_title']['hide_empty'] = 0;
	$handler->display->display_options['fields']['biblio_secondary_title']['empty_zero'] = 0;
	$handler->display->display_options['fields']['biblio_secondary_title']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['biblio_secondary_title']['biblio_label'] = 1;
	/* Field: Biblio: Notes */
	$handler->display->display_options['fields']['biblio_notes']['id'] = 'biblio_notes';
	$handler->display->display_options['fields']['biblio_notes']['table'] = 'biblio';
	$handler->display->display_options['fields']['biblio_notes']['field'] = 'biblio_notes';
	$handler->display->display_options['fields']['biblio_notes']['label'] = 'description';
	$handler->display->display_options['fields']['biblio_notes']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['biblio_notes']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['biblio_notes']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['biblio_notes']['alter']['external'] = 0;
	$handler->display->display_options['fields']['biblio_notes']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['biblio_notes']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['biblio_notes']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['biblio_notes']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['biblio_notes']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['biblio_notes']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['biblio_notes']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['biblio_notes']['alter']['html'] = 0;
	$handler->display->display_options['fields']['biblio_notes']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['biblio_notes']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['biblio_notes']['hide_empty'] = 0;
	$handler->display->display_options['fields']['biblio_notes']['empty_zero'] = 0;
	$handler->display->display_options['fields']['biblio_notes']['hide_alter_empty'] = 1;
	/* Field: Biblio: Keyword */
	$handler->display->display_options['fields']['word']['id'] = 'word';
	$handler->display->display_options['fields']['word']['table'] = 'biblio_keyword_data';
	$handler->display->display_options['fields']['word']['field'] = 'word';
	$handler->display->display_options['fields']['word']['label'] = 'subject';
	$handler->display->display_options['fields']['word']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['word']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['word']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['word']['alter']['external'] = 0;
	$handler->display->display_options['fields']['word']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['word']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['word']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['word']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['word']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['word']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['word']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['word']['alter']['html'] = 0;
	$handler->display->display_options['fields']['word']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['word']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['word']['hide_empty'] = 0;
	$handler->display->display_options['fields']['word']['empty_zero'] = 0;
	$handler->display->display_options['fields']['word']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['word']['link_to_keyword'] = 0;
	/* Field: Biblio: Publication Language */
	$handler->display->display_options['fields']['biblio_lang']['id'] = 'biblio_lang';
	$handler->display->display_options['fields']['biblio_lang']['table'] = 'biblio';
	$handler->display->display_options['fields']['biblio_lang']['field'] = 'biblio_lang';
	$handler->display->display_options['fields']['biblio_lang']['label'] = 'language';
	$handler->display->display_options['fields']['biblio_lang']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['biblio_lang']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['biblio_lang']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['biblio_lang']['alter']['external'] = 0;
	$handler->display->display_options['fields']['biblio_lang']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['biblio_lang']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['biblio_lang']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['biblio_lang']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['biblio_lang']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['biblio_lang']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['biblio_lang']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['biblio_lang']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['biblio_lang']['alter']['html'] = 0;
	$handler->display->display_options['fields']['biblio_lang']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['biblio_lang']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['biblio_lang']['hide_empty'] = 0;
	$handler->display->display_options['fields']['biblio_lang']['empty_zero'] = 0;
	$handler->display->display_options['fields']['biblio_lang']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['biblio_lang']['biblio_label'] = 1;
	/* Field: Biblio: Type of Publication */
	$handler->display->display_options['fields']['name']['id'] = 'name';
	$handler->display->display_options['fields']['name']['table'] = 'biblio_types';
	$handler->display->display_options['fields']['name']['field'] = 'name';
	$handler->display->display_options['fields']['name']['label'] = 'type';
	$handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['name']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['name']['alter']['external'] = 0;
	$handler->display->display_options['fields']['name']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['name']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['name']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['name']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['name']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['name']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['name']['alter']['html'] = 0;
	$handler->display->display_options['fields']['name']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['name']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['name']['hide_empty'] = 0;
	$handler->display->display_options['fields']['name']['empty_zero'] = 0;
	$handler->display->display_options['fields']['name']['hide_alter_empty'] = 1;
	/* Field: Biblio: Date Published */
	$handler->display->display_options['fields']['biblio_date']['id'] = 'biblio_date';
	$handler->display->display_options['fields']['biblio_date']['table'] = 'biblio';
	$handler->display->display_options['fields']['biblio_date']['field'] = 'biblio_date';
	$handler->display->display_options['fields']['biblio_date']['label'] = 'date';
	$handler->display->display_options['fields']['biblio_date']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['biblio_date']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['biblio_date']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['biblio_date']['alter']['external'] = 0;
	$handler->display->display_options['fields']['biblio_date']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['biblio_date']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['biblio_date']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['biblio_date']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['biblio_date']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['biblio_date']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['biblio_date']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['biblio_date']['alter']['html'] = 0;
	$handler->display->display_options['fields']['biblio_date']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['biblio_date']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['biblio_date']['hide_empty'] = 0;
	$handler->display->display_options['fields']['biblio_date']['empty_zero'] = 0;
	$handler->display->display_options['fields']['biblio_date']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['biblio_date']['biblio_label'] = 1;
	/* Filter criterion: Content: Published */
	$handler->display->display_options['filters']['status']['id'] = 'status';
	$handler->display->display_options['filters']['status']['table'] = 'node';
	$handler->display->display_options['filters']['status']['field'] = 'status';
	$handler->display->display_options['filters']['status']['value'] = 1;
	$handler->display->display_options['filters']['status']['group'] = 0;
	$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
	/* Filter criterion: Content: Type */
	$handler->display->display_options['filters']['type']['id'] = 'type';
	$handler->display->display_options['filters']['type']['table'] = 'node';
	$handler->display->display_options['filters']['type']['field'] = 'type';
	$handler->display->display_options['filters']['type']['value'] = array(
  'biblio' => 'biblio',
	);

	/* Display: Page */
	$handler = $view->new_display('page', 'Page', 'page');
	$handler->display->display_options['path'] = 'dwca-export/reference/page';

	/* Display: Data export */
	$handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
	$handler->display->display_options['pager']['type'] = 'some';
	$handler->display->display_options['style_plugin'] = 'views_data_export_csv';
	$handler->display->display_options['path'] = 'dwca-export/reference/csv';

	return $view;
}

function _view_image() {
	
	$view = new view;
	$view->name = 'dwca_export_image';
	$view->description = 'DwC-A image extension for a Taxon';
	$view->tag = 'default';
	$view->base_table = 'file_managed';
	$view->human_name = 'DwC-A Export Image';
	$view->core = 7;
	$view->api_version = '3.0';
	$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
	
	/* Display: Master */
	$handler = $view->new_display('default', 'Master', 'default');
	$handler->display->display_options['title'] = 'image_test';
	$handler->display->display_options['access']['type'] = 'perm';
	$handler->display->display_options['cache']['type'] = 'none';
	$handler->display->display_options['query']['type'] = 'views_query';
	$handler->display->display_options['query']['options']['query_comment'] = FALSE;
	$handler->display->display_options['exposed_form']['type'] = 'basic';
	$handler->display->display_options['pager']['type'] = 'full';
	$handler->display->display_options['pager']['options']['items_per_page'] = '10';
	$handler->display->display_options['style_plugin'] = 'table';
	$handler->display->display_options['style_options']['columns'] = array(
	  'filename' => 'filename',
	  'tid' => 'tid',
	);
	$handler->display->display_options['style_options']['default'] = '-1';
	$handler->display->display_options['style_options']['info'] = array(
	  'filename' => array(
	    'sortable' => 0,
	    'default_sort_order' => 'asc',
	    'align' => '',
	    'separator' => '',
	    'empty_column' => 0,
	),
	  'tid' => array(
	    'sortable' => 0,
	    'default_sort_order' => 'asc',
	    'align' => '',
	    'separator' => '',
	    'empty_column' => 0,
	),
	);
	$handler->display->display_options['style_options']['override'] = 1;
	$handler->display->display_options['style_options']['sticky'] = 0;
	$handler->display->display_options['style_options']['empty_table'] = 0;
	/* Relationship: File Usage: Taxonomy Term */
	$handler->display->display_options['relationships']['file_to_taxonomy_term']['id'] = 'file_to_taxonomy_term';
	$handler->display->display_options['relationships']['file_to_taxonomy_term']['table'] = 'file_usage';
	$handler->display->display_options['relationships']['file_to_taxonomy_term']['field'] = 'file_to_taxonomy_term';
	$handler->display->display_options['relationships']['file_to_taxonomy_term']['required'] = 0;
	/* Field: Taxonomy term: Term ID */
	$handler->display->display_options['fields']['tid']['id'] = 'tid';
	$handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
	$handler->display->display_options['fields']['tid']['field'] = 'tid';
	$handler->display->display_options['fields']['tid']['relationship'] = 'file_to_taxonomy_term';
	$handler->display->display_options['fields']['tid']['label'] = 'id';
	$handler->display->display_options['fields']['tid']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['external'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['html'] = 0;
	$handler->display->display_options['fields']['tid']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['tid']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['tid']['hide_empty'] = 0;
	$handler->display->display_options['fields']['tid']['empty_zero'] = 0;
	$handler->display->display_options['fields']['tid']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['tid']['format_plural'] = 0;
	/* Field: File: Name */
	$handler->display->display_options['fields']['filename']['id'] = 'filename';
	$handler->display->display_options['fields']['filename']['table'] = 'file_managed';
	$handler->display->display_options['fields']['filename']['field'] = 'filename';
	$handler->display->display_options['fields']['filename']['label'] = 'identifier';
	$handler->display->display_options['fields']['filename']['alter']['alter_text'] = 1;
	$handler->display->display_options['fields']['filename']['alter']['text'] = '[filename]';
	$handler->display->display_options['fields']['filename']['alter']['make_link'] = 1;
	$handler->display->display_options['fields']['filename']['alter']['absolute'] = 1;
	$handler->display->display_options['fields']['filename']['alter']['external'] = 0;
	$handler->display->display_options['fields']['filename']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['filename']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['filename']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['filename']['alter']['word_boundary'] = 0;
	$handler->display->display_options['fields']['filename']['alter']['ellipsis'] = 0;
	$handler->display->display_options['fields']['filename']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['filename']['alter']['strip_tags'] = 1;
	$handler->display->display_options['fields']['filename']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['filename']['alter']['html'] = 0;
	$handler->display->display_options['fields']['filename']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['filename']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['filename']['hide_empty'] = 0;
	$handler->display->display_options['fields']['filename']['empty_zero'] = 0;
	$handler->display->display_options['fields']['filename']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['filename']['link_to_file'] = 1;
	/* Field: File: Name */
	$handler->display->display_options['fields']['filename_1']['id'] = 'filename_1';
	$handler->display->display_options['fields']['filename_1']['table'] = 'file_managed';
	$handler->display->display_options['fields']['filename_1']['field'] = 'filename';
	$handler->display->display_options['fields']['filename_1']['label'] = 'title';
	$handler->display->display_options['fields']['filename_1']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['filename_1']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['filename_1']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['filename_1']['alter']['external'] = 0;
	$handler->display->display_options['fields']['filename_1']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['filename_1']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['filename_1']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['filename_1']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['filename_1']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['filename_1']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['filename_1']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['filename_1']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['filename_1']['alter']['html'] = 0;
	$handler->display->display_options['fields']['filename_1']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['filename_1']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['filename_1']['hide_empty'] = 0;
	$handler->display->display_options['fields']['filename_1']['empty_zero'] = 0;
	$handler->display->display_options['fields']['filename_1']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['filename_1']['link_to_file'] = 0;
	/* Field: File: Extension */
	$handler->display->display_options['fields']['extension']['id'] = 'extension';
	$handler->display->display_options['fields']['extension']['table'] = 'file_managed';
	$handler->display->display_options['fields']['extension']['field'] = 'extension';
	$handler->display->display_options['fields']['extension']['label'] = 'format';
	$handler->display->display_options['fields']['extension']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['extension']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['extension']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['extension']['alter']['external'] = 0;
	$handler->display->display_options['fields']['extension']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['extension']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['extension']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['extension']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['extension']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['extension']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['extension']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['extension']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['extension']['alter']['html'] = 0;
	$handler->display->display_options['fields']['extension']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['extension']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['extension']['hide_empty'] = 0;
	$handler->display->display_options['fields']['extension']['empty_zero'] = 0;
	$handler->display->display_options['fields']['extension']['hide_alter_empty'] = 1;
	/* Field: File: Licence */
	$handler->display->display_options['fields']['field_cc_licence']['id'] = 'field_cc_licence';
	$handler->display->display_options['fields']['field_cc_licence']['table'] = 'field_data_field_cc_licence';
	$handler->display->display_options['fields']['field_cc_licence']['field'] = 'field_cc_licence';
	$handler->display->display_options['fields']['field_cc_licence']['label'] = 'licence';
	$handler->display->display_options['fields']['field_cc_licence']['alter']['alter_text'] = 1;
	$handler->display->display_options['fields']['field_cc_licence']['alter']['text'] = 'Creative commons';
	$handler->display->display_options['fields']['field_cc_licence']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_cc_licence']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_cc_licence']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_cc_licence']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_cc_licence']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_cc_licence']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_cc_licence']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_cc_licence']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_cc_licence']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_cc_licence']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_cc_licence']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_cc_licence']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_cc_licence']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_cc_licence']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_cc_licence']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_cc_licence']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_cc_licence']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_cc_licence']['field_api_classes'] = 0;
	/* Filter criterion: File: Type */
	$handler->display->display_options['filters']['type']['id'] = 'type';
	$handler->display->display_options['filters']['type']['table'] = 'file_managed';
	$handler->display->display_options['filters']['type']['field'] = 'type';
	$handler->display->display_options['filters']['type']['value'] = array(
	  'image' => 'image',
	);
	/* Filter criterion: Taxonomy term: Term */
	$handler->display->display_options['filters']['tid']['id'] = 'tid';
	$handler->display->display_options['filters']['tid']['table'] = 'taxonomy_term_data';
	$handler->display->display_options['filters']['tid']['field'] = 'tid';
	$handler->display->display_options['filters']['tid']['relationship'] = 'file_to_taxonomy_term';
	$handler->display->display_options['filters']['tid']['operator'] = 'not empty';
	
	/* Display: Page */
	$handler = $view->new_display('page', 'Page', 'page');
	$handler->display->display_options['path'] = 'dwca-export/image/page';
	
	/* Display: Data export */
	$handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
	$handler->display->display_options['pager']['type'] = 'some';
	$handler->display->display_options['style_plugin'] = 'views_data_export_csv';
	$handler->display->display_options['path'] = 'dwca-export/image/csv';
	
	return $view;
}

function _view_specimen() {
  
	$view = new view;
	$view->name = 'dwca_export_specimen';
	$view->description = 'DwC-A specimen extension for a Taxon';
	$view->tag = 'default';
	$view->base_table = 'node';
	$view->human_name = 'DwC-A Export Specimen';
	$view->core = 7;
	$view->api_version = '3.0';
	$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
	
	/* Display: Master */
	$handler = $view->new_display('default', 'Master', 'default');
	$handler->display->display_options['access']['type'] = 'perm';
	$handler->display->display_options['cache']['type'] = 'none';
	$handler->display->display_options['query']['type'] = 'views_query';
	$handler->display->display_options['query']['options']['query_comment'] = FALSE;
	$handler->display->display_options['exposed_form']['type'] = 'basic';
	$handler->display->display_options['pager']['type'] = 'full';
	$handler->display->display_options['style_plugin'] = 'table';
	$handler->display->display_options['style_options']['columns'] = array(
	  'title' => 'title',
	);
	$handler->display->display_options['style_options']['default'] = '-1';
	$handler->display->display_options['style_options']['info'] = array(
	  'title' => array(
	    'sortable' => 0,
	    'default_sort_order' => 'asc',
	    'align' => '',
	    'separator' => '',
	),
	);
	$handler->display->display_options['style_options']['override'] = 1;
	$handler->display->display_options['style_options']['sticky'] = 0;
	$handler->display->display_options['style_options']['empty_table'] = 0;
	/* Relationship: Content: Taxonomy terms on node */
	$handler->display->display_options['relationships']['term_node_tid']['id'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['table'] = 'node';
	$handler->display->display_options['relationships']['term_node_tid']['field'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['required'] = 1;
	$handler->display->display_options['relationships']['term_node_tid']['vocabularies'] = array(
	  'dwca_export_test_taxonomy' => 'dwca_export_test_taxonomy',
	  'forums' => 0,
	  'tags' => 0,
	);
	/* Field: Taxonomy term: Term ID */
	$handler->display->display_options['fields']['tid']['id'] = 'tid';
	$handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
	$handler->display->display_options['fields']['tid']['field'] = 'tid';
	$handler->display->display_options['fields']['tid']['relationship'] = 'term_node_tid';
	$handler->display->display_options['fields']['tid']['label'] = 'id';
	$handler->display->display_options['fields']['tid']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['external'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['html'] = 0;
	$handler->display->display_options['fields']['tid']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['tid']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['tid']['hide_empty'] = 0;
	$handler->display->display_options['fields']['tid']['empty_zero'] = 0;
	$handler->display->display_options['fields']['tid']['hide_alter_empty'] = 0;
	$handler->display->display_options['fields']['tid']['format_plural'] = 0;
	/* Field: Content: Type status */
	$handler->display->display_options['fields']['field_type_status_1']['id'] = 'field_type_status_1';
	$handler->display->display_options['fields']['field_type_status_1']['table'] = 'field_data_field_type_status';
	$handler->display->display_options['fields']['field_type_status_1']['field'] = 'field_type_status';
	$handler->display->display_options['fields']['field_type_status_1']['label'] = 'typeStatus';
	$handler->display->display_options['fields']['field_type_status_1']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_type_status_1']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_type_status_1']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_type_status_1']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_type_status_1']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_type_status_1']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_type_status_1']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_type_status_1']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_type_status_1']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_type_status_1']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_type_status_1']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_type_status_1']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_type_status_1']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_type_status_1']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_type_status_1']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_type_status_1']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_type_status_1']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_type_status_1']['hide_alter_empty'] = 0;
	$handler->display->display_options['fields']['field_type_status_1']['field_api_classes'] = 0;
	/* Field: Content: Institution code */
	$handler->display->display_options['fields']['field_institution_code']['id'] = 'field_institution_code';
	$handler->display->display_options['fields']['field_institution_code']['table'] = 'field_data_field_institution_code';
	$handler->display->display_options['fields']['field_institution_code']['field'] = 'field_institution_code';
	$handler->display->display_options['fields']['field_institution_code']['label'] = 'institutionCode';
	$handler->display->display_options['fields']['field_institution_code']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_institution_code']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_institution_code']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_institution_code']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_institution_code']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_institution_code']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_institution_code']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_institution_code']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_institution_code']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_institution_code']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_institution_code']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_institution_code']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_institution_code']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_institution_code']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_institution_code']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_institution_code']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_institution_code']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_institution_code']['hide_alter_empty'] = 0;
	$handler->display->display_options['fields']['field_institution_code']['field_api_classes'] = 0;
	/* Field: Content: Collection code */
	$handler->display->display_options['fields']['field_collection_code']['id'] = 'field_collection_code';
	$handler->display->display_options['fields']['field_collection_code']['table'] = 'field_data_field_collection_code';
	$handler->display->display_options['fields']['field_collection_code']['field'] = 'field_collection_code';
	$handler->display->display_options['fields']['field_collection_code']['label'] = 'collectionCode';
	$handler->display->display_options['fields']['field_collection_code']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_collection_code']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_collection_code']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_collection_code']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_collection_code']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_collection_code']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_collection_code']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_collection_code']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_collection_code']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_collection_code']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_collection_code']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_collection_code']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_collection_code']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_collection_code']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_collection_code']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_collection_code']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_collection_code']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_collection_code']['hide_alter_empty'] = 0;
	$handler->display->display_options['fields']['field_collection_code']['field_api_classes'] = 0;
	/* Field: Content: Catalogue number */
	$handler->display->display_options['fields']['field_catalogue_number']['id'] = 'field_catalogue_number';
	$handler->display->display_options['fields']['field_catalogue_number']['table'] = 'field_data_field_catalogue_number';
	$handler->display->display_options['fields']['field_catalogue_number']['field'] = 'field_catalogue_number';
	$handler->display->display_options['fields']['field_catalogue_number']['label'] = 'catalogNumber';
	$handler->display->display_options['fields']['field_catalogue_number']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_catalogue_number']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_catalogue_number']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_catalogue_number']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_catalogue_number']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_catalogue_number']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_catalogue_number']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_catalogue_number']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_catalogue_number']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_catalogue_number']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_catalogue_number']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_catalogue_number']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_catalogue_number']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_catalogue_number']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_catalogue_number']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_catalogue_number']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_catalogue_number']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_catalogue_number']['hide_alter_empty'] = 0;
	$handler->display->display_options['fields']['field_catalogue_number']['field_api_classes'] = 0;
	/* Sort criterion: Content: Post date */
	$handler->display->display_options['sorts']['created']['id'] = 'created';
	$handler->display->display_options['sorts']['created']['table'] = 'node';
	$handler->display->display_options['sorts']['created']['field'] = 'created';
	$handler->display->display_options['sorts']['created']['order'] = 'DESC';
	/* Filter criterion: Content: Published */
	$handler->display->display_options['filters']['status']['id'] = 'status';
	$handler->display->display_options['filters']['status']['table'] = 'node';
	$handler->display->display_options['filters']['status']['field'] = 'status';
	$handler->display->display_options['filters']['status']['value'] = 1;
	$handler->display->display_options['filters']['status']['group'] = 0;
	$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
	/* Filter criterion: Content: Type */
	$handler->display->display_options['filters']['type']['id'] = 'type';
	$handler->display->display_options['filters']['type']['table'] = 'node';
	$handler->display->display_options['filters']['type']['field'] = 'type';
	$handler->display->display_options['filters']['type']['value'] = array(
	  'specimen_observation' => 'specimen_observation',
	);
	
	/* Display: Page */
	$handler = $view->new_display('page', 'Page', 'page');
	$handler->display->display_options['path'] = 'dwca_export/specimen/page';
	
	/* Display: Data export */
	$handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
	$handler->display->display_options['pager']['type'] = 'some';
	$handler->display->display_options['style_plugin'] = 'views_data_export_csv';
	$handler->display->display_options['path'] = 'dwca_export/specimen/csv';
	
 
  return $view; 
}

function _view_distribution() {

	$view = new view;
	$view->name = 'dwca_export_distribution';
	$view->description = '';
	$view->tag = 'default';
	$view->base_table = 'node';
	$view->human_name = 'DwC-A Distribution';
	$view->core = 7;
	$view->api_version = '3.0';
	$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

	/* Display: Master */
	$handler = $view->new_display('default', 'Master', 'default');
	$handler->display->display_options['title'] = 'DwC-A Distribution';
	$handler->display->display_options['access']['type'] = 'perm';
	$handler->display->display_options['cache']['type'] = 'none';
	$handler->display->display_options['query']['type'] = 'views_query';
	$handler->display->display_options['query']['options']['query_comment'] = FALSE;
	$handler->display->display_options['exposed_form']['type'] = 'basic';
	$handler->display->display_options['pager']['type'] = 'full';
	$handler->display->display_options['pager']['options']['items_per_page'] = '10';
	$handler->display->display_options['style_plugin'] = 'default';
	$handler->display->display_options['row_plugin'] = 'node';
	/* Relationship: Content: Taxonomy terms on node */
	$handler->display->display_options['relationships']['term_node_tid']['id'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['table'] = 'node';
	$handler->display->display_options['relationships']['term_node_tid']['field'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['required'] = 0;
	$handler->display->display_options['relationships']['term_node_tid']['vocabularies'] = array(
  'cichorieae' => 0,
  'image_keywords' => 0,
  'imaging_technique' => 0,
  'preparation_technique' => 0,
	);
	
	/* Field: Taxonomy term: Term ID */
	$handler->display->display_options['fields']['tid']['id'] = 'tid';
	$handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
	$handler->display->display_options['fields']['tid']['field'] = 'tid';
	$handler->display->display_options['fields']['tid']['relationship'] = 'term_node_tid';
	$handler->display->display_options['fields']['tid']['label'] = 'id';
	$handler->display->display_options['fields']['tid']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['external'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['html'] = 0;
	$handler->display->display_options['fields']['tid']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['tid']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['tid']['hide_empty'] = 0;
	$handler->display->display_options['fields']['tid']['empty_zero'] = 0;
	$handler->display->display_options['fields']['tid']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['tid']['format_plural'] = 0;
	/* Field: Content: Distribution */
	$handler->display->display_options['fields']['field_distribution']['id'] = 'field_distribution';
	$handler->display->display_options['fields']['field_distribution']['table'] = 'field_data_field_distribution';
	$handler->display->display_options['fields']['field_distribution']['field'] = 'field_distribution';
	$handler->display->display_options['fields']['field_distribution']['label'] = 'locationID';
	$handler->display->display_options['fields']['field_distribution']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_distribution']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_distribution']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_distribution']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_distribution']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_distribution']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_distribution']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_distribution']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_distribution']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_distribution']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_distribution']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_distribution']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_distribution']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_distribution']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_distribution']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_distribution']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_distribution']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_distribution']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_distribution']['field_api_classes'] = 0;
	/* Sort criterion: Content: Post date */
	$handler->display->display_options['sorts']['created']['id'] = 'created';
	$handler->display->display_options['sorts']['created']['table'] = 'node';
	$handler->display->display_options['sorts']['created']['field'] = 'created';
	$handler->display->display_options['sorts']['created']['order'] = 'DESC';
	/* Filter criterion: Content: Published */
	$handler->display->display_options['filters']['status']['id'] = 'status';
	$handler->display->display_options['filters']['status']['table'] = 'node';
	$handler->display->display_options['filters']['status']['field'] = 'status';
	$handler->display->display_options['filters']['status']['value'] = 1;
	$handler->display->display_options['filters']['status']['group'] = 1;
	$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
	/* Filter criterion: Content: Distribution (field_distribution) */
	$handler->display->display_options['filters']['field_distribution_value']['id'] = 'field_distribution_value';
	$handler->display->display_options['filters']['field_distribution_value']['table'] = 'field_data_field_distribution';
	$handler->display->display_options['filters']['field_distribution_value']['field'] = 'field_distribution_value';
	$handler->display->display_options['filters']['field_distribution_value']['operator'] = 'not empty';
	/* Filter criterion: Taxonomy term: Term */
	$handler->display->display_options['filters']['tid']['id'] = 'tid';
	$handler->display->display_options['filters']['tid']['table'] = 'taxonomy_term_data';
	$handler->display->display_options['filters']['tid']['field'] = 'tid';
	$handler->display->display_options['filters']['tid']['relationship'] = 'term_node_tid';
	$handler->display->display_options['filters']['tid']['operator'] = 'not empty';

	/* Display: Page */
	$handler = $view->new_display('page', 'Page', 'page');
	$handler->display->display_options['path'] = 'dwca-export/distribution/page';

	/* Display: Data export */
	$handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
	$handler->display->display_options['pager']['type'] = 'some';
	$handler->display->display_options['style_plugin'] = 'views_data_export_csv';
	$handler->display->display_options['path'] = 'dwca-export/distribution/csv';

	return $view;
}

function _view_description() {

	$view = new view;
	$view->name = 'dwca_export_description';
	$view->description = 'DwC-A description extension for a Taxon';
	$view->tag = 'default';
	$view->base_table = 'node';
	$view->human_name = 'DwC-A Export Description';
	$view->core = 7;
	$view->api_version = '3.0';
	$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

	/* Display: Master */
	$handler = $view->new_display('default', 'Master', 'default');
	$handler->display->display_options['title'] = 'Description Export';
	$handler->display->display_options['access']['type'] = 'perm';
	$handler->display->display_options['cache']['type'] = 'none';
	$handler->display->display_options['query']['type'] = 'views_query';
	$handler->display->display_options['query']['options']['query_comment'] = FALSE;
	$handler->display->display_options['exposed_form']['type'] = 'basic';
	$handler->display->display_options['pager']['type'] = 'full';
	$handler->display->display_options['pager']['options']['items_per_page'] = '10';
	$handler->display->display_options['style_plugin'] = 'table';
	$handler->display->display_options['style_options']['columns'] = array(
	  'title' => 'title',
	);
	$handler->display->display_options['style_options']['default'] = '-1';
	$handler->display->display_options['style_options']['info'] = array(
	  'title' => array(
	    'sortable' => 0,
	    'default_sort_order' => 'asc',
	    'align' => '',
	    'separator' => '',
	    'empty_column' => 0,
	),
	);
	$handler->display->display_options['style_options']['override'] = 1;
	$handler->display->display_options['style_options']['sticky'] = 0;
	$handler->display->display_options['style_options']['empty_table'] = 0;
	/* Relationship: Content: Taxonomy terms on node */
	$handler->display->display_options['relationships']['term_node_tid']['id'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['table'] = 'node';
	$handler->display->display_options['relationships']['term_node_tid']['field'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['required'] = 0;
	$handler->display->display_options['relationships']['term_node_tid']['vocabularies'] = array(
	  'cichorieae' => 0,
	  'image_keywords' => 0,
	  'imaging_technique' => 0,
	  'preparation_technique' => 0,
	);
	/* Field: Taxonomy term: Term ID */
	$handler->display->display_options['fields']['tid']['id'] = 'tid';
	$handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
	$handler->display->display_options['fields']['tid']['field'] = 'tid';
	$handler->display->display_options['fields']['tid']['relationship'] = 'term_node_tid';
	$handler->display->display_options['fields']['tid']['label'] = 'id';
	$handler->display->display_options['fields']['tid']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['external'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['html'] = 0;
	$handler->display->display_options['fields']['tid']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['tid']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['tid']['hide_empty'] = 0;
	$handler->display->display_options['fields']['tid']['empty_zero'] = 0;
	$handler->display->display_options['fields']['tid']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['tid']['format_plural'] = 0;
	/* Field: Content: Behaviour */
	$handler->display->display_options['fields']['field_behavious']['id'] = 'field_behavious';
	$handler->display->display_options['fields']['field_behavious']['table'] = 'field_data_field_behavious';
	$handler->display->display_options['fields']['field_behavious']['field'] = 'field_behavious';
	$handler->display->display_options['fields']['field_behavious']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_behavious']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_behavious']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_behavious']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_behavious']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_behavious']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_behavious']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_behavious']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_behavious']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_behavious']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_behavious']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_behavious']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_behavious']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_behavious']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_behavious']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_behavious']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_behavious']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_behavious']['field_api_classes'] = 0;
	/* Field: Content: Biology */
	$handler->display->display_options['fields']['field_biology']['id'] = 'field_biology';
	$handler->display->display_options['fields']['field_biology']['table'] = 'field_data_field_biology';
	$handler->display->display_options['fields']['field_biology']['field'] = 'field_biology';
	$handler->display->display_options['fields']['field_biology']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_biology']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_biology']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_biology']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_biology']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_biology']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_biology']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_biology']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_biology']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_biology']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_biology']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_biology']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_biology']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_biology']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_biology']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_biology']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_biology']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_biology']['field_api_classes'] = 0;
	/* Field: Content: Conservation status */
	$handler->display->display_options['fields']['field_conservation_status']['id'] = 'field_conservation_status';
	$handler->display->display_options['fields']['field_conservation_status']['table'] = 'field_data_field_conservation_status';
	$handler->display->display_options['fields']['field_conservation_status']['field'] = 'field_conservation_status';
	$handler->display->display_options['fields']['field_conservation_status']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_conservation_status']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_conservation_status']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_conservation_status']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_conservation_status']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_conservation_status']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_conservation_status']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_conservation_status']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_conservation_status']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_conservation_status']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_conservation_status']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_conservation_status']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_conservation_status']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_conservation_status']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_conservation_status']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_conservation_status']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_conservation_status']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_conservation_status']['field_api_classes'] = 0;
	/* Field: Content: General description */
	$handler->display->display_options['fields']['field_general_description']['id'] = 'field_general_description';
	$handler->display->display_options['fields']['field_general_description']['table'] = 'field_data_field_general_description';
	$handler->display->display_options['fields']['field_general_description']['field'] = 'field_general_description';
	$handler->display->display_options['fields']['field_general_description']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_general_description']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_general_description']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_general_description']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_general_description']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_general_description']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_general_description']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_general_description']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_general_description']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_general_description']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_general_description']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_general_description']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_general_description']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_general_description']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_general_description']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_general_description']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_general_description']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_general_description']['field_api_classes'] = 0;
	/* Field: Content: Legislation */
	$handler->display->display_options['fields']['field_legislation']['id'] = 'field_legislation';
	$handler->display->display_options['fields']['field_legislation']['table'] = 'field_data_field_legislation';
	$handler->display->display_options['fields']['field_legislation']['field'] = 'field_legislation';
	$handler->display->display_options['fields']['field_legislation']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_legislation']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_legislation']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_legislation']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_legislation']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_legislation']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_legislation']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_legislation']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_legislation']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_legislation']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_legislation']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_legislation']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_legislation']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_legislation']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_legislation']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_legislation']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_legislation']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_legislation']['field_api_classes'] = 0;
	/* Field: Content: Management */
	$handler->display->display_options['fields']['field_management']['id'] = 'field_management';
	$handler->display->display_options['fields']['field_management']['table'] = 'field_data_field_management';
	$handler->display->display_options['fields']['field_management']['field'] = 'field_management';
	$handler->display->display_options['fields']['field_management']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_management']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_management']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_management']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_management']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_management']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_management']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_management']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_management']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_management']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_management']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_management']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_management']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_management']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_management']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_management']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_management']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_management']['field_api_classes'] = 0;
	/* Field: Content: Procedures */
	$handler->display->display_options['fields']['field_procedures']['id'] = 'field_procedures';
	$handler->display->display_options['fields']['field_procedures']['table'] = 'field_data_field_procedures';
	$handler->display->display_options['fields']['field_procedures']['field'] = 'field_procedures';
	$handler->display->display_options['fields']['field_procedures']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_procedures']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_procedures']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_procedures']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_procedures']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_procedures']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_procedures']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_procedures']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_procedures']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_procedures']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_procedures']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_procedures']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_procedures']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_procedures']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_procedures']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_procedures']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_procedures']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_procedures']['field_api_classes'] = 0;
	/* Field: Content: Threats */
	$handler->display->display_options['fields']['field_threats']['id'] = 'field_threats';
	$handler->display->display_options['fields']['field_threats']['table'] = 'field_data_field_threats';
	$handler->display->display_options['fields']['field_threats']['field'] = 'field_threats';
	$handler->display->display_options['fields']['field_threats']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_threats']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_threats']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_threats']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_threats']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_threats']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_threats']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_threats']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_threats']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_threats']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_threats']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_threats']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_threats']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_threats']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_threats']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_threats']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_threats']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_threats']['field_api_classes'] = 0;
	/* Field: Content: Trends */
	$handler->display->display_options['fields']['field_trends']['id'] = 'field_trends';
	$handler->display->display_options['fields']['field_trends']['table'] = 'field_data_field_trends';
	$handler->display->display_options['fields']['field_trends']['field'] = 'field_trends';
	$handler->display->display_options['fields']['field_trends']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_trends']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_trends']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_trends']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_trends']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_trends']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_trends']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_trends']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_trends']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_trends']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_trends']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_trends']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_trends']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_trends']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_trends']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_trends']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_trends']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_trends']['field_api_classes'] = 0;
	/* Field: Content: Associations */
	$handler->display->display_options['fields']['field_associations']['id'] = 'field_associations';
	$handler->display->display_options['fields']['field_associations']['table'] = 'field_data_field_associations';
	$handler->display->display_options['fields']['field_associations']['field'] = 'field_associations';
	$handler->display->display_options['fields']['field_associations']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_associations']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_associations']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_associations']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_associations']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_associations']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_associations']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_associations']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_associations']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_associations']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_associations']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_associations']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_associations']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_associations']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_associations']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_associations']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_associations']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_associations']['field_api_classes'] = 0;
	/* Field: Content: Behaviour */
	$handler->display->display_options['fields']['field_behavious_1']['id'] = 'field_behavious_1';
	$handler->display->display_options['fields']['field_behavious_1']['table'] = 'field_data_field_behavious';
	$handler->display->display_options['fields']['field_behavious_1']['field'] = 'field_behavious';
	$handler->display->display_options['fields']['field_behavious_1']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_behavious_1']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_behavious_1']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_behavious_1']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_behavious_1']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_behavious_1']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_behavious_1']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_behavious_1']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_behavious_1']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_behavious_1']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_behavious_1']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_behavious_1']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_behavious_1']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_behavious_1']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_behavious_1']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_behavious_1']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_behavious_1']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_behavious_1']['field_api_classes'] = 0;
	/* Field: Content: Cyclicity */
	$handler->display->display_options['fields']['field_cyclicity']['id'] = 'field_cyclicity';
	$handler->display->display_options['fields']['field_cyclicity']['table'] = 'field_data_field_cyclicity';
	$handler->display->display_options['fields']['field_cyclicity']['field'] = 'field_cyclicity';
	$handler->display->display_options['fields']['field_cyclicity']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_cyclicity']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_cyclicity']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_cyclicity']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_cyclicity']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_cyclicity']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_cyclicity']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_cyclicity']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_cyclicity']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_cyclicity']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_cyclicity']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_cyclicity']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_cyclicity']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_cyclicity']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_cyclicity']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_cyclicity']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_cyclicity']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_cyclicity']['field_api_classes'] = 0;
	/* Field: Content: Cytology */
	$handler->display->display_options['fields']['field_cytology']['id'] = 'field_cytology';
	$handler->display->display_options['fields']['field_cytology']['table'] = 'field_data_field_cytology';
	$handler->display->display_options['fields']['field_cytology']['field'] = 'field_cytology';
	$handler->display->display_options['fields']['field_cytology']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_cytology']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_cytology']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_cytology']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_cytology']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_cytology']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_cytology']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_cytology']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_cytology']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_cytology']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_cytology']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_cytology']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_cytology']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_cytology']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_cytology']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_cytology']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_cytology']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_cytology']['field_api_classes'] = 0;
	/* Field: Content: Diagnostic description */
	$handler->display->display_options['fields']['field_diagnostic_description']['id'] = 'field_diagnostic_description';
	$handler->display->display_options['fields']['field_diagnostic_description']['table'] = 'field_data_field_diagnostic_description';
	$handler->display->display_options['fields']['field_diagnostic_description']['field'] = 'field_diagnostic_description';
	$handler->display->display_options['fields']['field_diagnostic_description']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_diagnostic_description']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_diagnostic_description']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_diagnostic_description']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_diagnostic_description']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_diagnostic_description']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_diagnostic_description']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_diagnostic_description']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_diagnostic_description']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_diagnostic_description']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_diagnostic_description']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_diagnostic_description']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_diagnostic_description']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_diagnostic_description']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_diagnostic_description']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_diagnostic_description']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_diagnostic_description']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_diagnostic_description']['field_api_classes'] = 0;
	/* Field: Content: Diseases */
	$handler->display->display_options['fields']['field_diseases']['id'] = 'field_diseases';
	$handler->display->display_options['fields']['field_diseases']['table'] = 'field_data_field_diseases';
	$handler->display->display_options['fields']['field_diseases']['field'] = 'field_diseases';
	$handler->display->display_options['fields']['field_diseases']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_diseases']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_diseases']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_diseases']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_diseases']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_diseases']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_diseases']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_diseases']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_diseases']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_diseases']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_diseases']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_diseases']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_diseases']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_diseases']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_diseases']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_diseases']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_diseases']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_diseases']['field_api_classes'] = 0;
	/* Field: Content: Dispersal */
	$handler->display->display_options['fields']['field_dispersal']['id'] = 'field_dispersal';
	$handler->display->display_options['fields']['field_dispersal']['table'] = 'field_data_field_dispersal';
	$handler->display->display_options['fields']['field_dispersal']['field'] = 'field_dispersal';
	$handler->display->display_options['fields']['field_dispersal']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_dispersal']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_dispersal']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_dispersal']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_dispersal']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_dispersal']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_dispersal']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_dispersal']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_dispersal']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_dispersal']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_dispersal']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_dispersal']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_dispersal']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_dispersal']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_dispersal']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_dispersal']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_dispersal']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_dispersal']['field_api_classes'] = 0;
	/* Field: Content: Distribution */
	$handler->display->display_options['fields']['field_distribution']['id'] = 'field_distribution';
	$handler->display->display_options['fields']['field_distribution']['table'] = 'field_data_field_distribution';
	$handler->display->display_options['fields']['field_distribution']['field'] = 'field_distribution';
	$handler->display->display_options['fields']['field_distribution']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_distribution']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_distribution']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_distribution']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_distribution']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_distribution']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_distribution']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_distribution']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_distribution']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_distribution']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_distribution']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_distribution']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_distribution']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_distribution']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_distribution']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_distribution']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_distribution']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_distribution']['field_api_classes'] = 0;
	/* Field: Content: Ecology */
	$handler->display->display_options['fields']['field_ecology']['id'] = 'field_ecology';
	$handler->display->display_options['fields']['field_ecology']['table'] = 'field_data_field_ecology';
	$handler->display->display_options['fields']['field_ecology']['field'] = 'field_ecology';
	$handler->display->display_options['fields']['field_ecology']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_ecology']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_ecology']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_ecology']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_ecology']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_ecology']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_ecology']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_ecology']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_ecology']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_ecology']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_ecology']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_ecology']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_ecology']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_ecology']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_ecology']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_ecology']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_ecology']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_ecology']['field_api_classes'] = 0;
	/* Field: Content: Evolution */
	$handler->display->display_options['fields']['field_evolution']['id'] = 'field_evolution';
	$handler->display->display_options['fields']['field_evolution']['table'] = 'field_data_field_evolution';
	$handler->display->display_options['fields']['field_evolution']['field'] = 'field_evolution';
	$handler->display->display_options['fields']['field_evolution']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_evolution']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_evolution']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_evolution']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_evolution']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_evolution']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_evolution']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_evolution']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_evolution']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_evolution']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_evolution']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_evolution']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_evolution']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_evolution']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_evolution']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_evolution']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_evolution']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_evolution']['field_api_classes'] = 0;
	/* Field: Content: Genetics */
	$handler->display->display_options['fields']['field_genetics']['id'] = 'field_genetics';
	$handler->display->display_options['fields']['field_genetics']['table'] = 'field_data_field_genetics';
	$handler->display->display_options['fields']['field_genetics']['field'] = 'field_genetics';
	$handler->display->display_options['fields']['field_genetics']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_genetics']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_genetics']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_genetics']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_genetics']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_genetics']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_genetics']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_genetics']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_genetics']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_genetics']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_genetics']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_genetics']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_genetics']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_genetics']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_genetics']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_genetics']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_genetics']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_genetics']['field_api_classes'] = 0;
	/* Field: Content: Growth */
	$handler->display->display_options['fields']['field_growth']['id'] = 'field_growth';
	$handler->display->display_options['fields']['field_growth']['table'] = 'field_data_field_growth';
	$handler->display->display_options['fields']['field_growth']['field'] = 'field_growth';
	$handler->display->display_options['fields']['field_growth']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_growth']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_growth']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_growth']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_growth']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_growth']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_growth']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_growth']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_growth']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_growth']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_growth']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_growth']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_growth']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_growth']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_growth']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_growth']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_growth']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_growth']['field_api_classes'] = 0;
	/* Field: Content: Habitat */
	$handler->display->display_options['fields']['field_habitat']['id'] = 'field_habitat';
	$handler->display->display_options['fields']['field_habitat']['table'] = 'field_data_field_habitat';
	$handler->display->display_options['fields']['field_habitat']['field'] = 'field_habitat';
	$handler->display->display_options['fields']['field_habitat']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_habitat']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_habitat']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_habitat']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_habitat']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_habitat']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_habitat']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_habitat']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_habitat']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_habitat']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_habitat']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_habitat']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_habitat']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_habitat']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_habitat']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_habitat']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_habitat']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_habitat']['field_api_classes'] = 0;
	/* Field: Content: Life cycle */
	$handler->display->display_options['fields']['field_life_cycle']['id'] = 'field_life_cycle';
	$handler->display->display_options['fields']['field_life_cycle']['table'] = 'field_data_field_life_cycle';
	$handler->display->display_options['fields']['field_life_cycle']['field'] = 'field_life_cycle';
	$handler->display->display_options['fields']['field_life_cycle']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_life_cycle']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_life_cycle']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_life_cycle']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_life_cycle']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_life_cycle']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_life_cycle']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_life_cycle']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_life_cycle']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_life_cycle']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_life_cycle']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_life_cycle']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_life_cycle']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_life_cycle']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_life_cycle']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_life_cycle']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_life_cycle']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_life_cycle']['field_api_classes'] = 0;
	/* Field: Content: Life expectancy */
	$handler->display->display_options['fields']['field_life_expectancy']['id'] = 'field_life_expectancy';
	$handler->display->display_options['fields']['field_life_expectancy']['table'] = 'field_data_field_life_expectancy';
	$handler->display->display_options['fields']['field_life_expectancy']['field'] = 'field_life_expectancy';
	$handler->display->display_options['fields']['field_life_expectancy']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_life_expectancy']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_life_expectancy']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_life_expectancy']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_life_expectancy']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_life_expectancy']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_life_expectancy']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_life_expectancy']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_life_expectancy']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_life_expectancy']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_life_expectancy']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_life_expectancy']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_life_expectancy']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_life_expectancy']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_life_expectancy']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_life_expectancy']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_life_expectancy']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_life_expectancy']['field_api_classes'] = 0;
	/* Field: Content: Look alikes */
	$handler->display->display_options['fields']['field_look_alikes']['id'] = 'field_look_alikes';
	$handler->display->display_options['fields']['field_look_alikes']['table'] = 'field_data_field_look_alikes';
	$handler->display->display_options['fields']['field_look_alikes']['field'] = 'field_look_alikes';
	$handler->display->display_options['fields']['field_look_alikes']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_look_alikes']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_look_alikes']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_look_alikes']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_look_alikes']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_look_alikes']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_look_alikes']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_look_alikes']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_look_alikes']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_look_alikes']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_look_alikes']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_look_alikes']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_look_alikes']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_look_alikes']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_look_alikes']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_look_alikes']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_look_alikes']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_look_alikes']['field_api_classes'] = 0;
	/* Field: Content: Migration */
	$handler->display->display_options['fields']['field_migration']['id'] = 'field_migration';
	$handler->display->display_options['fields']['field_migration']['table'] = 'field_data_field_migration';
	$handler->display->display_options['fields']['field_migration']['field'] = 'field_migration';
	$handler->display->display_options['fields']['field_migration']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_migration']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_migration']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_migration']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_migration']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_migration']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_migration']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_migration']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_migration']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_migration']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_migration']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_migration']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_migration']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_migration']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_migration']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_migration']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_migration']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_migration']['field_api_classes'] = 0;
	/* Field: Content: Molecular biology */
	$handler->display->display_options['fields']['field_molecular_biology']['id'] = 'field_molecular_biology';
	$handler->display->display_options['fields']['field_molecular_biology']['table'] = 'field_data_field_molecular_biology';
	$handler->display->display_options['fields']['field_molecular_biology']['field'] = 'field_molecular_biology';
	$handler->display->display_options['fields']['field_molecular_biology']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_molecular_biology']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_molecular_biology']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_molecular_biology']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_molecular_biology']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_molecular_biology']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_molecular_biology']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_molecular_biology']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_molecular_biology']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_molecular_biology']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_molecular_biology']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_molecular_biology']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_molecular_biology']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_molecular_biology']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_molecular_biology']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_molecular_biology']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_molecular_biology']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_molecular_biology']['field_api_classes'] = 0;
	/* Field: Content: Morphology */
	$handler->display->display_options['fields']['field_morphology']['id'] = 'field_morphology';
	$handler->display->display_options['fields']['field_morphology']['table'] = 'field_data_field_morphology';
	$handler->display->display_options['fields']['field_morphology']['field'] = 'field_morphology';
	$handler->display->display_options['fields']['field_morphology']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_morphology']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_morphology']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_morphology']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_morphology']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_morphology']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_morphology']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_morphology']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_morphology']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_morphology']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_morphology']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_morphology']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_morphology']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_morphology']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_morphology']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_morphology']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_morphology']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_morphology']['field_api_classes'] = 0;
	/* Field: Content: Physiology */
	$handler->display->display_options['fields']['field_physiology']['id'] = 'field_physiology';
	$handler->display->display_options['fields']['field_physiology']['table'] = 'field_data_field_physiology';
	$handler->display->display_options['fields']['field_physiology']['field'] = 'field_physiology';
	$handler->display->display_options['fields']['field_physiology']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_physiology']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_physiology']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_physiology']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_physiology']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_physiology']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_physiology']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_physiology']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_physiology']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_physiology']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_physiology']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_physiology']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_physiology']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_physiology']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_physiology']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_physiology']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_physiology']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_physiology']['field_api_classes'] = 0;
	/* Field: Content: Population biology */
	$handler->display->display_options['fields']['field_population_biology']['id'] = 'field_population_biology';
	$handler->display->display_options['fields']['field_population_biology']['table'] = 'field_data_field_population_biology';
	$handler->display->display_options['fields']['field_population_biology']['field'] = 'field_population_biology';
	$handler->display->display_options['fields']['field_population_biology']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_population_biology']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_population_biology']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_population_biology']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_population_biology']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_population_biology']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_population_biology']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_population_biology']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_population_biology']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_population_biology']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_population_biology']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_population_biology']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_population_biology']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_population_biology']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_population_biology']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_population_biology']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_population_biology']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_population_biology']['field_api_classes'] = 0;
	/* Field: Content: Reproduction */
	$handler->display->display_options['fields']['field_reproduction']['id'] = 'field_reproduction';
	$handler->display->display_options['fields']['field_reproduction']['table'] = 'field_data_field_reproduction';
	$handler->display->display_options['fields']['field_reproduction']['field'] = 'field_reproduction';
	$handler->display->display_options['fields']['field_reproduction']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_reproduction']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_reproduction']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_reproduction']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_reproduction']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_reproduction']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_reproduction']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_reproduction']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_reproduction']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_reproduction']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_reproduction']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_reproduction']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_reproduction']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_reproduction']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_reproduction']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_reproduction']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_reproduction']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_reproduction']['field_api_classes'] = 0;
	/* Field: Content: Risk statement */
	$handler->display->display_options['fields']['field_risk_statement']['id'] = 'field_risk_statement';
	$handler->display->display_options['fields']['field_risk_statement']['table'] = 'field_data_field_risk_statement';
	$handler->display->display_options['fields']['field_risk_statement']['field'] = 'field_risk_statement';
	$handler->display->display_options['fields']['field_risk_statement']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_risk_statement']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_risk_statement']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_risk_statement']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_risk_statement']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_risk_statement']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_risk_statement']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_risk_statement']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_risk_statement']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_risk_statement']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_risk_statement']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_risk_statement']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_risk_statement']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_risk_statement']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_risk_statement']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_risk_statement']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_risk_statement']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_risk_statement']['field_api_classes'] = 0;
	/* Field: Content: Size */
	$handler->display->display_options['fields']['field_size']['id'] = 'field_size';
	$handler->display->display_options['fields']['field_size']['table'] = 'field_data_field_size';
	$handler->display->display_options['fields']['field_size']['field'] = 'field_size';
	$handler->display->display_options['fields']['field_size']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_size']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_size']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_size']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_size']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_size']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_size']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_size']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_size']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_size']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_size']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_size']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_size']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_size']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_size']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_size']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_size']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_size']['field_api_classes'] = 0;
	/* Field: Content: Taxon biology */
	$handler->display->display_options['fields']['field_taxon_biology']['id'] = 'field_taxon_biology';
	$handler->display->display_options['fields']['field_taxon_biology']['table'] = 'field_data_field_taxon_biology';
	$handler->display->display_options['fields']['field_taxon_biology']['field'] = 'field_taxon_biology';
	$handler->display->display_options['fields']['field_taxon_biology']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_taxon_biology']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_taxon_biology']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_taxon_biology']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_taxon_biology']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_taxon_biology']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_taxon_biology']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_taxon_biology']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_taxon_biology']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_taxon_biology']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_taxon_biology']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_taxon_biology']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_taxon_biology']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_taxon_biology']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_taxon_biology']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_taxon_biology']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_taxon_biology']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_taxon_biology']['field_api_classes'] = 0;
	/* Field: Content: Trophic strategy */
	$handler->display->display_options['fields']['field_trophic_strategy']['id'] = 'field_trophic_strategy';
	$handler->display->display_options['fields']['field_trophic_strategy']['table'] = 'field_data_field_trophic_strategy';
	$handler->display->display_options['fields']['field_trophic_strategy']['field'] = 'field_trophic_strategy';
	$handler->display->display_options['fields']['field_trophic_strategy']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_trophic_strategy']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_trophic_strategy']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_trophic_strategy']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_trophic_strategy']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_trophic_strategy']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_trophic_strategy']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_trophic_strategy']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_trophic_strategy']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_trophic_strategy']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_trophic_strategy']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_trophic_strategy']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_trophic_strategy']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_trophic_strategy']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_trophic_strategy']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_trophic_strategy']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_trophic_strategy']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_trophic_strategy']['field_api_classes'] = 0;
	/* Field: Content: Uses */
	$handler->display->display_options['fields']['field_uses']['id'] = 'field_uses';
	$handler->display->display_options['fields']['field_uses']['table'] = 'field_data_field_uses';
	$handler->display->display_options['fields']['field_uses']['field'] = 'field_uses';
	$handler->display->display_options['fields']['field_uses']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_uses']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_uses']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_uses']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_uses']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_uses']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_uses']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_uses']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_uses']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_uses']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_uses']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_uses']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_uses']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_uses']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_uses']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_uses']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_uses']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_uses']['field_api_classes'] = 0;
	/* Sort criterion: Content: Post date */
	$handler->display->display_options['sorts']['created']['id'] = 'created';
	$handler->display->display_options['sorts']['created']['table'] = 'node';
	$handler->display->display_options['sorts']['created']['field'] = 'created';
	$handler->display->display_options['sorts']['created']['order'] = 'DESC';
	/* Filter criterion: Content: Published */
	$handler->display->display_options['filters']['status']['id'] = 'status';
	$handler->display->display_options['filters']['status']['table'] = 'node';
	$handler->display->display_options['filters']['status']['field'] = 'status';
	$handler->display->display_options['filters']['status']['value'] = 1;
	$handler->display->display_options['filters']['status']['group'] = 0;
	$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
	/* Filter criterion: Content: Type */
	$handler->display->display_options['filters']['type']['id'] = 'type';
	$handler->display->display_options['filters']['type']['table'] = 'node';
	$handler->display->display_options['filters']['type']['field'] = 'type';
	$handler->display->display_options['filters']['type']['value'] = array(
	  'spm' => 'spm',
	);

	/* Display: Page */
	$handler = $view->new_display('page', 'Page', 'page');
	$handler->display->display_options['path'] = 'dwca-export/description/page';

	/* Display: Data export */
	$handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
	$handler->display->display_options['pager']['type'] = 'some';
	$handler->display->display_options['style_plugin'] = 'views_data_export_csv';
	$handler->display->display_options['path'] = 'dwca-export/description/csv';

	return $view;
}

function _view_description_general() {
	
	$view = new view;
	$view->name = 'dwca_export_description_general';
	$view->description = '';
	$view->tag = 'default';
	$view->base_table = 'node';
	$view->human_name = 'DwC-A Description General';
	$view->core = 7;
	$view->api_version = '3.0';
	$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
	
	/* Display: Master */
	$handler = $view->new_display('default', 'Master', 'default');
	$handler->display->display_options['title'] = 'DwC-A Description General';
	$handler->display->display_options['access']['type'] = 'perm';
	$handler->display->display_options['cache']['type'] = 'none';
	$handler->display->display_options['query']['type'] = 'views_query';
	$handler->display->display_options['query']['options']['query_comment'] = FALSE;
	$handler->display->display_options['exposed_form']['type'] = 'basic';
	$handler->display->display_options['pager']['type'] = 'full';
	$handler->display->display_options['pager']['options']['items_per_page'] = '10';
	$handler->display->display_options['style_plugin'] = 'default';
	$handler->display->display_options['row_plugin'] = 'node';
	/* Relationship: Content: Taxonomy terms on node */
	$handler->display->display_options['relationships']['term_node_tid']['id'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['table'] = 'node';
	$handler->display->display_options['relationships']['term_node_tid']['field'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['required'] = 0;
	$handler->display->display_options['relationships']['term_node_tid']['vocabularies'] = array(
	  'cichorieae' => 0,
	  'image_keywords' => 0,
	  'imaging_technique' => 0,
	  'preparation_technique' => 0,
	);
	/* Field: Taxonomy term: Term ID */
	$handler->display->display_options['fields']['tid']['id'] = 'tid';
	$handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
	$handler->display->display_options['fields']['tid']['field'] = 'tid';
	$handler->display->display_options['fields']['tid']['relationship'] = 'term_node_tid';
	$handler->display->display_options['fields']['tid']['label'] = 'id';
	$handler->display->display_options['fields']['tid']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['external'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['html'] = 0;
	$handler->display->display_options['fields']['tid']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['tid']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['tid']['hide_empty'] = 0;
	$handler->display->display_options['fields']['tid']['empty_zero'] = 0;
	$handler->display->display_options['fields']['tid']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['tid']['format_plural'] = 0;
	/* Field: Content: General description */
	$handler->display->display_options['fields']['field_general_description']['id'] = 'field_general_description';
	$handler->display->display_options['fields']['field_general_description']['table'] = 'field_data_field_general_description';
	$handler->display->display_options['fields']['field_general_description']['field'] = 'field_general_description';
	$handler->display->display_options['fields']['field_general_description']['label'] = 'type';
	$handler->display->display_options['fields']['field_general_description']['alter']['alter_text'] = 1;
	$handler->display->display_options['fields']['field_general_description']['alter']['text'] = 'general';
	$handler->display->display_options['fields']['field_general_description']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_general_description']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_general_description']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_general_description']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_general_description']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_general_description']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_general_description']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_general_description']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_general_description']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_general_description']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_general_description']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_general_description']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_general_description']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_general_description']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_general_description']['empty'] = 'general';
	$handler->display->display_options['fields']['field_general_description']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_general_description']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_general_description']['hide_alter_empty'] = 0;
	$handler->display->display_options['fields']['field_general_description']['field_api_classes'] = 0;
	/* Field: Content: General description */
	$handler->display->display_options['fields']['field_general_description_1']['id'] = 'field_general_description_1';
	$handler->display->display_options['fields']['field_general_description_1']['table'] = 'field_data_field_general_description';
	$handler->display->display_options['fields']['field_general_description_1']['field'] = 'field_general_description';
	$handler->display->display_options['fields']['field_general_description_1']['label'] = 'description';
	$handler->display->display_options['fields']['field_general_description_1']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_general_description_1']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_general_description_1']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_general_description_1']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_general_description_1']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_general_description_1']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_general_description_1']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_general_description_1']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_general_description_1']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_general_description_1']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_general_description_1']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_general_description_1']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_general_description_1']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_general_description_1']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_general_description_1']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_general_description_1']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_general_description_1']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_general_description_1']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_general_description_1']['field_api_classes'] = 0;
	/* Sort criterion: Content: Post date */
	$handler->display->display_options['sorts']['created']['id'] = 'created';
	$handler->display->display_options['sorts']['created']['table'] = 'node';
	$handler->display->display_options['sorts']['created']['field'] = 'created';
	$handler->display->display_options['sorts']['created']['order'] = 'DESC';
	/* Filter criterion: Content: Published */
	$handler->display->display_options['filters']['status']['id'] = 'status';
	$handler->display->display_options['filters']['status']['table'] = 'node';
	$handler->display->display_options['filters']['status']['field'] = 'status';
	$handler->display->display_options['filters']['status']['value'] = 1;
	$handler->display->display_options['filters']['status']['group'] = 1;
	$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
	/* Filter criterion: Content: Type */
	$handler->display->display_options['filters']['type']['id'] = 'type';
	$handler->display->display_options['filters']['type']['table'] = 'node';
	$handler->display->display_options['filters']['type']['field'] = 'type';
	$handler->display->display_options['filters']['type']['value'] = array(
	  'spm' => 'spm',
	);
	
	/* Display: Page */
	$handler = $view->new_display('page', 'Page', 'page');
	$handler->display->display_options['path'] = 'dwca-export/description-general/page';
	
	/* Display: Data export */
	$handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
	$handler->display->display_options['pager']['type'] = 'some';
	$handler->display->display_options['style_plugin'] = 'views_data_export_csv';
	$handler->display->display_options['path'] = 'dwca-export/description-general/csv';
	
	return $view;
}

function _view_description_morphology() {

	$view = new view;
	$view->name = 'dwca_export_description_morph';
	$view->description = '';
	$view->tag = 'default';
	$view->base_table = 'node';
	$view->human_name = 'DwC-A Description Morphology';
	$view->core = 7;
	$view->api_version = '3.0';
	$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
	
	/* Display: Master */
	$handler = $view->new_display('default', 'Master', 'default');
	$handler->display->display_options['title'] = 'DwC-A Description Morphology';
	$handler->display->display_options['access']['type'] = 'perm';
	$handler->display->display_options['cache']['type'] = 'none';
	$handler->display->display_options['query']['type'] = 'views_query';
	$handler->display->display_options['query']['options']['query_comment'] = FALSE;
	$handler->display->display_options['exposed_form']['type'] = 'basic';
	$handler->display->display_options['pager']['type'] = 'full';
	$handler->display->display_options['pager']['options']['items_per_page'] = '10';
	$handler->display->display_options['style_plugin'] = 'default';
	$handler->display->display_options['row_plugin'] = 'node';
	/* Relationship: Content: Taxonomy terms on node */
	$handler->display->display_options['relationships']['term_node_tid']['id'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['table'] = 'node';
	$handler->display->display_options['relationships']['term_node_tid']['field'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['required'] = 0;
	$handler->display->display_options['relationships']['term_node_tid']['vocabularies'] = array(
	  'cichorieae' => 0,
	  'image_keywords' => 0,
	  'imaging_technique' => 0,
	  'preparation_technique' => 0,
	);
	/* Field: Taxonomy term: Term ID */
	$handler->display->display_options['fields']['tid']['id'] = 'tid';
	$handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
	$handler->display->display_options['fields']['tid']['field'] = 'tid';
	$handler->display->display_options['fields']['tid']['relationship'] = 'term_node_tid';
	$handler->display->display_options['fields']['tid']['label'] = 'id';
	$handler->display->display_options['fields']['tid']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['external'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['html'] = 0;
	$handler->display->display_options['fields']['tid']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['tid']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['tid']['hide_empty'] = 0;
	$handler->display->display_options['fields']['tid']['empty_zero'] = 0;
	$handler->display->display_options['fields']['tid']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['tid']['format_plural'] = 0;
	/* Field: Content: Morphology */
	$handler->display->display_options['fields']['field_morphology']['id'] = 'field_morphology';
	$handler->display->display_options['fields']['field_morphology']['table'] = 'field_data_field_morphology';
	$handler->display->display_options['fields']['field_morphology']['field'] = 'field_morphology';
	$handler->display->display_options['fields']['field_morphology']['label'] = 'type';
	$handler->display->display_options['fields']['field_morphology']['alter']['alter_text'] = 1;
	$handler->display->display_options['fields']['field_morphology']['alter']['text'] = 'morphology';
	$handler->display->display_options['fields']['field_morphology']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_morphology']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_morphology']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_morphology']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_morphology']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_morphology']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_morphology']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_morphology']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_morphology']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_morphology']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_morphology']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_morphology']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_morphology']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_morphology']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_morphology']['empty'] = 'morphology';
	$handler->display->display_options['fields']['field_morphology']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_morphology']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_morphology']['hide_alter_empty'] = 0;
	$handler->display->display_options['fields']['field_morphology']['field_api_classes'] = 0;
	/* Field: Content: Morphology */
	$handler->display->display_options['fields']['field_morphology_1']['id'] = 'field_morphology_1';
	$handler->display->display_options['fields']['field_morphology_1']['table'] = 'field_data_field_morphology';
	$handler->display->display_options['fields']['field_morphology_1']['field'] = 'field_morphology';
	$handler->display->display_options['fields']['field_morphology_1']['label'] = 'description';
	$handler->display->display_options['fields']['field_morphology_1']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_morphology_1']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_morphology_1']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_morphology_1']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_morphology_1']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_morphology_1']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_morphology_1']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_morphology_1']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_morphology_1']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_morphology_1']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_morphology_1']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_morphology_1']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_morphology_1']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_morphology_1']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_morphology_1']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_morphology_1']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_morphology_1']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_morphology_1']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_morphology_1']['field_api_classes'] = 0;
	/* Sort criterion: Content: Post date */
	$handler->display->display_options['sorts']['created']['id'] = 'created';
	$handler->display->display_options['sorts']['created']['table'] = 'node';
	$handler->display->display_options['sorts']['created']['field'] = 'created';
	$handler->display->display_options['sorts']['created']['order'] = 'DESC';
	/* Filter criterion: Content: Published */
	$handler->display->display_options['filters']['status']['id'] = 'status';
	$handler->display->display_options['filters']['status']['table'] = 'node';
	$handler->display->display_options['filters']['status']['field'] = 'status';
	$handler->display->display_options['filters']['status']['value'] = 1;
	$handler->display->display_options['filters']['status']['group'] = 1;
	$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
	/* Filter criterion: Content: Type */
	$handler->display->display_options['filters']['type']['id'] = 'type';
	$handler->display->display_options['filters']['type']['table'] = 'node';
	$handler->display->display_options['filters']['type']['field'] = 'type';
	$handler->display->display_options['filters']['type']['value'] = array(
	  'spm' => 'spm',
	);
	
	/* Display: Page */
	$handler = $view->new_display('page', 'Page', 'page');
	$handler->display->display_options['path'] = 'dwca-export/description-morphology/page';
	
	/* Display: Data export */
	$handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
	$handler->display->display_options['pager']['type'] = 'some';
	$handler->display->display_options['style_plugin'] = 'views_data_export_csv';
	$handler->display->display_options['path'] = 'dwca-export/description-morphology/csv';	
	
	return $view;
}

function _view_description_behaviour() {

	$view = new view;
	$view->name = 'dwca_export_description_behav';
	$view->description = '';
	$view->tag = 'default';
	$view->base_table = 'node';
	$view->human_name = 'DwC-A Description Behaviour';
	$view->core = 7;
	$view->api_version = '3.0';
	$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

	/* Display: Master */
	$handler = $view->new_display('default', 'Master', 'default');
	$handler->display->display_options['title'] = 'DwC-A Description behaviour';
	$handler->display->display_options['access']['type'] = 'perm';
	$handler->display->display_options['cache']['type'] = 'none';
	$handler->display->display_options['query']['type'] = 'views_query';
	$handler->display->display_options['query']['options']['query_comment'] = FALSE;
	$handler->display->display_options['exposed_form']['type'] = 'basic';
	$handler->display->display_options['pager']['type'] = 'full';
	$handler->display->display_options['pager']['options']['items_per_page'] = '10';
	$handler->display->display_options['style_plugin'] = 'default';
	$handler->display->display_options['row_plugin'] = 'node';
	/* Relationship: Content: Taxonomy terms on node */
	$handler->display->display_options['relationships']['term_node_tid']['id'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['table'] = 'node';
	$handler->display->display_options['relationships']['term_node_tid']['field'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['required'] = 0;
	$handler->display->display_options['relationships']['term_node_tid']['vocabularies'] = array(
  'cichorieae' => 0,
  'image_keywords' => 0,
  'imaging_technique' => 0,
  'preparation_technique' => 0,
	);
	/* Field: Taxonomy term: Term ID */
	$handler->display->display_options['fields']['tid']['id'] = 'tid';
	$handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
	$handler->display->display_options['fields']['tid']['field'] = 'tid';
	$handler->display->display_options['fields']['tid']['relationship'] = 'term_node_tid';
	$handler->display->display_options['fields']['tid']['label'] = 'id';
	$handler->display->display_options['fields']['tid']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['external'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['html'] = 0;
	$handler->display->display_options['fields']['tid']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['tid']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['tid']['hide_empty'] = 0;
	$handler->display->display_options['fields']['tid']['empty_zero'] = 0;
	$handler->display->display_options['fields']['tid']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['tid']['format_plural'] = 0;
	/* Field: Content: Behaviour */
	$handler->display->display_options['fields']['field_behavious_1']['id'] = 'field_behavious_1';
	$handler->display->display_options['fields']['field_behavious_1']['table'] = 'field_data_field_behavious';
	$handler->display->display_options['fields']['field_behavious_1']['field'] = 'field_behavious';
	$handler->display->display_options['fields']['field_behavious_1']['label'] = 'type';
	$handler->display->display_options['fields']['field_behavious_1']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_behavious_1']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_behavious_1']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_behavious_1']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_behavious_1']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_behavious_1']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_behavious_1']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_behavious_1']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_behavious_1']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_behavious_1']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_behavious_1']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_behavious_1']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_behavious_1']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_behavious_1']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_behavious_1']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_behavious_1']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_behavious_1']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_behavious_1']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_behavious_1']['field_api_classes'] = 0;
	/* Field: Content: Behaviour */
	$handler->display->display_options['fields']['field_behavious']['id'] = 'field_behavious';
	$handler->display->display_options['fields']['field_behavious']['table'] = 'field_data_field_behavious';
	$handler->display->display_options['fields']['field_behavious']['field'] = 'field_behavious';
	$handler->display->display_options['fields']['field_behavious']['label'] = 'description';
	$handler->display->display_options['fields']['field_behavious']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_behavious']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_behavious']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_behavious']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_behavious']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_behavious']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_behavious']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_behavious']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_behavious']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_behavious']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_behavious']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_behavious']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_behavious']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_behavious']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_behavious']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_behavious']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_behavious']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_behavious']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_behavious']['field_api_classes'] = 0;
	/* Sort criterion: Content: Post date */
	$handler->display->display_options['sorts']['created']['id'] = 'created';
	$handler->display->display_options['sorts']['created']['table'] = 'node';
	$handler->display->display_options['sorts']['created']['field'] = 'created';
	$handler->display->display_options['sorts']['created']['order'] = 'DESC';
	/* Filter criterion: Content: Published */
	$handler->display->display_options['filters']['status']['id'] = 'status';
	$handler->display->display_options['filters']['status']['table'] = 'node';
	$handler->display->display_options['filters']['status']['field'] = 'status';
	$handler->display->display_options['filters']['status']['value'] = 1;
	$handler->display->display_options['filters']['status']['group'] = 1;
	$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
	/* Filter criterion: Content: Type */
	$handler->display->display_options['filters']['type']['id'] = 'type';
	$handler->display->display_options['filters']['type']['table'] = 'node';
	$handler->display->display_options['filters']['type']['field'] = 'type';
	$handler->display->display_options['filters']['type']['value'] = array(
  'spm' => 'spm',
	);

	/* Display: Page */
	$handler = $view->new_display('page', 'Page', 'page');
	$handler->display->display_options['path'] = 'dwca-export/description-behaviour/page';

	/* Display: Data export */
	$handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
	$handler->display->display_options['pager']['type'] = 'some';
	$handler->display->display_options['style_plugin'] = 'views_data_export_csv';
	$handler->display->display_options['path'] = 'dwca-export/description-behaviour/csv';


	return $view;
}

function _view_description_biology() {

	$view = new view;
	$view->name = 'dwca_export_description_biol';
	$view->description = '';
	$view->tag = 'default';
	$view->base_table = 'node';
	$view->human_name = 'DwC-A Description biology';
	$view->core = 7;
	$view->api_version = '3.0';
	$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

	/* Display: Master */
	$handler = $view->new_display('default', 'Master', 'default');
	$handler->display->display_options['title'] = 'DwC-A Description biology';
	$handler->display->display_options['access']['type'] = 'perm';
	$handler->display->display_options['cache']['type'] = 'none';
	$handler->display->display_options['query']['type'] = 'views_query';
	$handler->display->display_options['query']['options']['query_comment'] = FALSE;
	$handler->display->display_options['exposed_form']['type'] = 'basic';
	$handler->display->display_options['pager']['type'] = 'full';
	$handler->display->display_options['pager']['options']['items_per_page'] = '10';
	$handler->display->display_options['style_plugin'] = 'default';
	$handler->display->display_options['row_plugin'] = 'node';
	/* Relationship: Content: Taxonomy terms on node */
	$handler->display->display_options['relationships']['term_node_tid']['id'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['table'] = 'node';
	$handler->display->display_options['relationships']['term_node_tid']['field'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['required'] = 0;
	$handler->display->display_options['relationships']['term_node_tid']['vocabularies'] = array(
	  'cichorieae' => 0,
	  'image_keywords' => 0,
	  'imaging_technique' => 0,
	  'preparation_technique' => 0,
	);
	/* Field: Taxonomy term: Term ID */
	$handler->display->display_options['fields']['tid']['id'] = 'tid';
	$handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
	$handler->display->display_options['fields']['tid']['field'] = 'tid';
	$handler->display->display_options['fields']['tid']['relationship'] = 'term_node_tid';
	$handler->display->display_options['fields']['tid']['label'] = 'id';
	$handler->display->display_options['fields']['tid']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['external'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['html'] = 0;
	$handler->display->display_options['fields']['tid']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['tid']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['tid']['hide_empty'] = 0;
	$handler->display->display_options['fields']['tid']['empty_zero'] = 0;
	$handler->display->display_options['fields']['tid']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['tid']['format_plural'] = 0;
	/* Field: Content: biology */
	$handler->display->display_options['fields']['field_biology']['id'] = 'field_biology';
	$handler->display->display_options['fields']['field_biology']['table'] = 'field_data_field_biology';
	$handler->display->display_options['fields']['field_biology']['field'] = 'field_biology';
	$handler->display->display_options['fields']['field_biology']['label'] = 'type';
	$handler->display->display_options['fields']['field_biology']['alter']['alter_text'] = 1;
	$handler->display->display_options['fields']['field_biology']['alter']['text'] = 'biology';
	$handler->display->display_options['fields']['field_biology']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_biology']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_biology']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_biology']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_biology']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_biology']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_biology']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_biology']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_biology']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_biology']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_biology']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_biology']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_biology']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_biology']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_biology']['empty'] = 'biology';
	$handler->display->display_options['fields']['field_biology']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_biology']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_biology']['hide_alter_empty'] = 0;
	$handler->display->display_options['fields']['field_biology']['field_api_classes'] = 0;
	/* Field: Content: biology */
	$handler->display->display_options['fields']['field_biology_1']['id'] = 'field_biology_1';
	$handler->display->display_options['fields']['field_biology_1']['table'] = 'field_data_field_biology';
	$handler->display->display_options['fields']['field_biology_1']['field'] = 'field_biology';
	$handler->display->display_options['fields']['field_biology_1']['label'] = 'description';
	$handler->display->display_options['fields']['field_biology_1']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_biology_1']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_biology_1']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_biology_1']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_biology_1']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_biology_1']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_biology_1']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_biology_1']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_biology_1']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_biology_1']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_biology_1']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_biology_1']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_biology_1']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_biology_1']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_biology_1']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_biology_1']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_biology_1']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_biology_1']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_biology_1']['field_api_classes'] = 0;
	/* Sort criterion: Content: Post date */
	$handler->display->display_options['sorts']['created']['id'] = 'created';
	$handler->display->display_options['sorts']['created']['table'] = 'node';
	$handler->display->display_options['sorts']['created']['field'] = 'created';
	$handler->display->display_options['sorts']['created']['order'] = 'DESC';
	/* Filter criterion: Content: Published */
	$handler->display->display_options['filters']['status']['id'] = 'status';
	$handler->display->display_options['filters']['status']['table'] = 'node';
	$handler->display->display_options['filters']['status']['field'] = 'status';
	$handler->display->display_options['filters']['status']['value'] = 1;
	$handler->display->display_options['filters']['status']['group'] = 1;
	$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
	/* Filter criterion: Content: Type */
	$handler->display->display_options['filters']['type']['id'] = 'type';
	$handler->display->display_options['filters']['type']['table'] = 'node';
	$handler->display->display_options['filters']['type']['field'] = 'type';
	$handler->display->display_options['filters']['type']['value'] = array(
	  'spm' => 'spm',
	);

	/* Display: Page */
	$handler = $view->new_display('page', 'Page', 'page');
	$handler->display->display_options['path'] = 'dwca-export/description-biology/page';

	/* Display: Data export */
	$handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
	$handler->display->display_options['pager']['type'] = 'some';
	$handler->display->display_options['style_plugin'] = 'views_data_export_csv';
	$handler->display->display_options['path'] = 'dwca-export/description-biology/csv';

	return $view;
}

function _view_description_conservation_status() {

	$view = new view;
	$view->name = 'dwca_export_description_cons';
	$view->description = '';
	$view->tag = 'default';
	$view->base_table = 'node';
	$view->human_name = 'DwC-A Description conservation_status';
	$view->core = 7;
	$view->api_version = '3.0';
	$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

	/* Display: Master */
	$handler = $view->new_display('default', 'Master', 'default');
	$handler->display->display_options['title'] = 'DwC-A Description Conservation';
	$handler->display->display_options['access']['type'] = 'perm';
	$handler->display->display_options['cache']['type'] = 'none';
	$handler->display->display_options['query']['type'] = 'views_query';
	$handler->display->display_options['query']['options']['query_comment'] = FALSE;
	$handler->display->display_options['exposed_form']['type'] = 'basic';
	$handler->display->display_options['pager']['type'] = 'full';
	$handler->display->display_options['pager']['options']['items_per_page'] = '10';
	$handler->display->display_options['style_plugin'] = 'default';
	$handler->display->display_options['row_plugin'] = 'node';
	/* Relationship: Content: Taxonomy terms on node */
	$handler->display->display_options['relationships']['term_node_tid']['id'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['table'] = 'node';
	$handler->display->display_options['relationships']['term_node_tid']['field'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['required'] = 0;
	$handler->display->display_options['relationships']['term_node_tid']['vocabularies'] = array(
	  'cichorieae' => 0,
	  'image_keywords' => 0,
	  'imaging_technique' => 0,
	  'preparation_technique' => 0,
	);
	/* Field: Taxonomy term: Term ID */
	$handler->display->display_options['fields']['tid']['id'] = 'tid';
	$handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
	$handler->display->display_options['fields']['tid']['field'] = 'tid';
	$handler->display->display_options['fields']['tid']['relationship'] = 'term_node_tid';
	$handler->display->display_options['fields']['tid']['label'] = 'id';
	$handler->display->display_options['fields']['tid']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['external'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['html'] = 0;
	$handler->display->display_options['fields']['tid']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['tid']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['tid']['hide_empty'] = 0;
	$handler->display->display_options['fields']['tid']['empty_zero'] = 0;
	$handler->display->display_options['fields']['tid']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['tid']['format_plural'] = 0;
	/* Field: Content: conservation_status */
	$handler->display->display_options['fields']['field_conservation_status']['id'] = 'field_conservation_status';
	$handler->display->display_options['fields']['field_conservation_status']['table'] = 'field_data_field_conservation_status';
	$handler->display->display_options['fields']['field_conservation_status']['field'] = 'field_conservation_status';
	$handler->display->display_options['fields']['field_conservation_status']['label'] = 'type';
	$handler->display->display_options['fields']['field_conservation_status']['alter']['alter_text'] = 1;
	$handler->display->display_options['fields']['field_conservation_status']['alter']['text'] = 'conservation_status';
	$handler->display->display_options['fields']['field_conservation_status']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_conservation_status']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_conservation_status']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_conservation_status']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_conservation_status']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_conservation_status']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_conservation_status']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_conservation_status']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_conservation_status']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_conservation_status']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_conservation_status']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_conservation_status']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_conservation_status']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_conservation_status']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_conservation_status']['empty'] = 'conservation_status';
	$handler->display->display_options['fields']['field_conservation_status']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_conservation_status']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_conservation_status']['hide_alter_empty'] = 0;
	$handler->display->display_options['fields']['field_conservation_status']['field_api_classes'] = 0;
	/* Field: Content: conservation_status */
	$handler->display->display_options['fields']['field_conservation_status_1']['id'] = 'field_conservation_status_1';
	$handler->display->display_options['fields']['field_conservation_status_1']['table'] = 'field_data_field_conservation_status';
	$handler->display->display_options['fields']['field_conservation_status_1']['field'] = 'field_conservation_status';
	$handler->display->display_options['fields']['field_conservation_status_1']['label'] = 'description';
	$handler->display->display_options['fields']['field_conservation_status_1']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_conservation_status_1']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_conservation_status_1']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_conservation_status_1']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_conservation_status_1']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_conservation_status_1']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_conservation_status_1']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_conservation_status_1']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_conservation_status_1']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_conservation_status_1']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_conservation_status_1']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_conservation_status_1']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_conservation_status_1']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_conservation_status_1']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_conservation_status_1']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_conservation_status_1']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_conservation_status_1']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_conservation_status_1']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_conservation_status_1']['field_api_classes'] = 0;
	/* Sort criterion: Content: Post date */
	$handler->display->display_options['sorts']['created']['id'] = 'created';
	$handler->display->display_options['sorts']['created']['table'] = 'node';
	$handler->display->display_options['sorts']['created']['field'] = 'created';
	$handler->display->display_options['sorts']['created']['order'] = 'DESC';
	/* Filter criterion: Content: Published */
	$handler->display->display_options['filters']['status']['id'] = 'status';
	$handler->display->display_options['filters']['status']['table'] = 'node';
	$handler->display->display_options['filters']['status']['field'] = 'status';
	$handler->display->display_options['filters']['status']['value'] = 1;
	$handler->display->display_options['filters']['status']['group'] = 1;
	$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
	/* Filter criterion: Content: Type */
	$handler->display->display_options['filters']['type']['id'] = 'type';
	$handler->display->display_options['filters']['type']['table'] = 'node';
	$handler->display->display_options['filters']['type']['field'] = 'type';
	$handler->display->display_options['filters']['type']['value'] = array(
	  'spm' => 'spm',
	);

	/* Display: Page */
	$handler = $view->new_display('page', 'Page', 'page');
	$handler->display->display_options['path'] = 'dwca-export/description-conservation_status/page';

	/* Display: Data export */
	$handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
	$handler->display->display_options['pager']['type'] = 'some';
	$handler->display->display_options['style_plugin'] = 'views_data_export_csv';
	$handler->display->display_options['path'] = 'dwca-export/description-conservation_status/csv';

	return $view;
}

function _view_description_legislation() {

	$view = new view;
	$view->name = 'dwca_export_description_leg';
	$view->description = '';
	$view->tag = 'default';
	$view->base_table = 'node';
	$view->human_name = 'DwC-A Description legislation';
	$view->core = 7;
	$view->api_version = '3.0';
	$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

	/* Display: Master */
	$handler = $view->new_display('default', 'Master', 'default');
	$handler->display->display_options['title'] = 'DwC-A Description Legislation';
	$handler->display->display_options['access']['type'] = 'perm';
	$handler->display->display_options['cache']['type'] = 'none';
	$handler->display->display_options['query']['type'] = 'views_query';
	$handler->display->display_options['query']['options']['query_comment'] = FALSE;
	$handler->display->display_options['exposed_form']['type'] = 'basic';
	$handler->display->display_options['pager']['type'] = 'full';
	$handler->display->display_options['pager']['options']['items_per_page'] = '10';
	$handler->display->display_options['style_plugin'] = 'default';
	$handler->display->display_options['row_plugin'] = 'node';
	/* Relationship: Content: Taxonomy terms on node */
	$handler->display->display_options['relationships']['term_node_tid']['id'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['table'] = 'node';
	$handler->display->display_options['relationships']['term_node_tid']['field'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['required'] = 0;
	$handler->display->display_options['relationships']['term_node_tid']['vocabularies'] = array(
	  'cichorieae' => 0,
	  'image_keywords' => 0,
	  'imaging_technique' => 0,
	  'preparation_technique' => 0,
	);
	/* Field: Taxonomy term: Term ID */
	$handler->display->display_options['fields']['tid']['id'] = 'tid';
	$handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
	$handler->display->display_options['fields']['tid']['field'] = 'tid';
	$handler->display->display_options['fields']['tid']['relationship'] = 'term_node_tid';
	$handler->display->display_options['fields']['tid']['label'] = 'id';
	$handler->display->display_options['fields']['tid']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['external'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['html'] = 0;
	$handler->display->display_options['fields']['tid']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['tid']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['tid']['hide_empty'] = 0;
	$handler->display->display_options['fields']['tid']['empty_zero'] = 0;
	$handler->display->display_options['fields']['tid']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['tid']['format_plural'] = 0;
	/* Field: Content: legislation */
	$handler->display->display_options['fields']['field_legislation']['id'] = 'field_legislation';
	$handler->display->display_options['fields']['field_legislation']['table'] = 'field_data_field_legislation';
	$handler->display->display_options['fields']['field_legislation']['field'] = 'field_legislation';
	$handler->display->display_options['fields']['field_legislation']['label'] = 'type';
	$handler->display->display_options['fields']['field_legislation']['alter']['alter_text'] = 1;
	$handler->display->display_options['fields']['field_legislation']['alter']['text'] = 'legislation';
	$handler->display->display_options['fields']['field_legislation']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_legislation']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_legislation']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_legislation']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_legislation']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_legislation']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_legislation']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_legislation']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_legislation']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_legislation']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_legislation']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_legislation']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_legislation']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_legislation']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_legislation']['empty'] = 'legislation';
	$handler->display->display_options['fields']['field_legislation']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_legislation']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_legislation']['hide_alter_empty'] = 0;
	$handler->display->display_options['fields']['field_legislation']['field_api_classes'] = 0;
	/* Field: Content: legislation */
	$handler->display->display_options['fields']['field_legislation_1']['id'] = 'field_legislation_1';
	$handler->display->display_options['fields']['field_legislation_1']['table'] = 'field_data_field_legislation';
	$handler->display->display_options['fields']['field_legislation_1']['field'] = 'field_legislation';
	$handler->display->display_options['fields']['field_legislation_1']['label'] = 'description';
	$handler->display->display_options['fields']['field_legislation_1']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_legislation_1']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_legislation_1']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_legislation_1']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_legislation_1']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_legislation_1']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_legislation_1']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_legislation_1']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_legislation_1']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_legislation_1']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_legislation_1']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_legislation_1']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_legislation_1']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_legislation_1']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_legislation_1']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_legislation_1']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_legislation_1']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_legislation_1']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_legislation_1']['field_api_classes'] = 0;
	/* Sort criterion: Content: Post date */
	$handler->display->display_options['sorts']['created']['id'] = 'created';
	$handler->display->display_options['sorts']['created']['table'] = 'node';
	$handler->display->display_options['sorts']['created']['field'] = 'created';
	$handler->display->display_options['sorts']['created']['order'] = 'DESC';
	/* Filter criterion: Content: Published */
	$handler->display->display_options['filters']['status']['id'] = 'status';
	$handler->display->display_options['filters']['status']['table'] = 'node';
	$handler->display->display_options['filters']['status']['field'] = 'status';
	$handler->display->display_options['filters']['status']['value'] = 1;
	$handler->display->display_options['filters']['status']['group'] = 1;
	$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
	/* Filter criterion: Content: Type */
	$handler->display->display_options['filters']['type']['id'] = 'type';
	$handler->display->display_options['filters']['type']['table'] = 'node';
	$handler->display->display_options['filters']['type']['field'] = 'type';
	$handler->display->display_options['filters']['type']['value'] = array(
	  'spm' => 'spm',
	);

	/* Display: Page */
	$handler = $view->new_display('page', 'Page', 'page');
	$handler->display->display_options['path'] = 'dwca-export/description-legislation/page';

	/* Display: Data export */
	$handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
	$handler->display->display_options['pager']['type'] = 'some';
	$handler->display->display_options['style_plugin'] = 'views_data_export_csv';
	$handler->display->display_options['path'] = 'dwca-export/description-legislation/csv';

	return $view;
}

function _view_description_management() {

	$view = new view;
	$view->name = 'dwca_export_description_manage';
	$view->description = '';
	$view->tag = 'default';
	$view->base_table = 'node';
	$view->human_name = 'DwC-A Description management';
	$view->core = 7;
	$view->api_version = '3.0';
	$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

	/* Display: Master */
	$handler = $view->new_display('default', 'Master', 'default');
	$handler->display->display_options['title'] = 'DwC-A Description management';
	$handler->display->display_options['access']['type'] = 'perm';
	$handler->display->display_options['cache']['type'] = 'none';
	$handler->display->display_options['query']['type'] = 'views_query';
	$handler->display->display_options['query']['options']['query_comment'] = FALSE;
	$handler->display->display_options['exposed_form']['type'] = 'basic';
	$handler->display->display_options['pager']['type'] = 'full';
	$handler->display->display_options['pager']['options']['items_per_page'] = '10';
	$handler->display->display_options['style_plugin'] = 'default';
	$handler->display->display_options['row_plugin'] = 'node';
	/* Relationship: Content: Taxonomy terms on node */
	$handler->display->display_options['relationships']['term_node_tid']['id'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['table'] = 'node';
	$handler->display->display_options['relationships']['term_node_tid']['field'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['required'] = 0;
	$handler->display->display_options['relationships']['term_node_tid']['vocabularies'] = array(
	  'cichorieae' => 0,
	  'image_keywords' => 0,
	  'imaging_technique' => 0,
	  'preparation_technique' => 0,
	);
	/* Field: Taxonomy term: Term ID */
	$handler->display->display_options['fields']['tid']['id'] = 'tid';
	$handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
	$handler->display->display_options['fields']['tid']['field'] = 'tid';
	$handler->display->display_options['fields']['tid']['relationship'] = 'term_node_tid';
	$handler->display->display_options['fields']['tid']['label'] = 'id';
	$handler->display->display_options['fields']['tid']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['external'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['html'] = 0;
	$handler->display->display_options['fields']['tid']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['tid']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['tid']['hide_empty'] = 0;
	$handler->display->display_options['fields']['tid']['empty_zero'] = 0;
	$handler->display->display_options['fields']['tid']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['tid']['format_plural'] = 0;
	/* Field: Content: management */
	$handler->display->display_options['fields']['field_management']['id'] = 'field_management';
	$handler->display->display_options['fields']['field_management']['table'] = 'field_data_field_management';
	$handler->display->display_options['fields']['field_management']['field'] = 'field_management';
	$handler->display->display_options['fields']['field_management']['label'] = 'type';
	$handler->display->display_options['fields']['field_management']['alter']['alter_text'] = 1;
	$handler->display->display_options['fields']['field_management']['alter']['text'] = 'management';
	$handler->display->display_options['fields']['field_management']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_management']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_management']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_management']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_management']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_management']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_management']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_management']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_management']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_management']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_management']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_management']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_management']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_management']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_management']['empty'] = 'management';
	$handler->display->display_options['fields']['field_management']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_management']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_management']['hide_alter_empty'] = 0;
	$handler->display->display_options['fields']['field_management']['field_api_classes'] = 0;
	/* Field: Content: management */
	$handler->display->display_options['fields']['field_management_1']['id'] = 'field_management_1';
	$handler->display->display_options['fields']['field_management_1']['table'] = 'field_data_field_management';
	$handler->display->display_options['fields']['field_management_1']['field'] = 'field_management';
	$handler->display->display_options['fields']['field_management_1']['label'] = 'description';
	$handler->display->display_options['fields']['field_management_1']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_management_1']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_management_1']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_management_1']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_management_1']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_management_1']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_management_1']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_management_1']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_management_1']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_management_1']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_management_1']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_management_1']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_management_1']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_management_1']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_management_1']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_management_1']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_management_1']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_management_1']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_management_1']['field_api_classes'] = 0;
	/* Sort criterion: Content: Post date */
	$handler->display->display_options['sorts']['created']['id'] = 'created';
	$handler->display->display_options['sorts']['created']['table'] = 'node';
	$handler->display->display_options['sorts']['created']['field'] = 'created';
	$handler->display->display_options['sorts']['created']['order'] = 'DESC';
	/* Filter criterion: Content: Published */
	$handler->display->display_options['filters']['status']['id'] = 'status';
	$handler->display->display_options['filters']['status']['table'] = 'node';
	$handler->display->display_options['filters']['status']['field'] = 'status';
	$handler->display->display_options['filters']['status']['value'] = 1;
	$handler->display->display_options['filters']['status']['group'] = 1;
	$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
	/* Filter criterion: Content: Type */
	$handler->display->display_options['filters']['type']['id'] = 'type';
	$handler->display->display_options['filters']['type']['table'] = 'node';
	$handler->display->display_options['filters']['type']['field'] = 'type';
	$handler->display->display_options['filters']['type']['value'] = array(
	  'spm' => 'spm',
	);

	/* Display: Page */
	$handler = $view->new_display('page', 'Page', 'page');
	$handler->display->display_options['path'] = 'dwca-export/description-management/page';

	/* Display: Data export */
	$handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
	$handler->display->display_options['pager']['type'] = 'some';
	$handler->display->display_options['style_plugin'] = 'views_data_export_csv';
	$handler->display->display_options['path'] = 'dwca-export/description-management/csv';

	return $view;
}

function _view_description_procedures() {

	$view = new view;
	$view->name = 'dwca_export_description_proc';
	$view->description = '';
	$view->tag = 'default';
	$view->base_table = 'node';
	$view->human_name = 'DwC-A Description procedures';
	$view->core = 7;
	$view->api_version = '3.0';
	$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

	/* Display: Master */
	$handler = $view->new_display('default', 'Master', 'default');
	$handler->display->display_options['title'] = 'DwC-A Description procedures';
	$handler->display->display_options['access']['type'] = 'perm';
	$handler->display->display_options['cache']['type'] = 'none';
	$handler->display->display_options['query']['type'] = 'views_query';
	$handler->display->display_options['query']['options']['query_comment'] = FALSE;
	$handler->display->display_options['exposed_form']['type'] = 'basic';
	$handler->display->display_options['pager']['type'] = 'full';
	$handler->display->display_options['pager']['options']['items_per_page'] = '10';
	$handler->display->display_options['style_plugin'] = 'default';
	$handler->display->display_options['row_plugin'] = 'node';
	/* Relationship: Content: Taxonomy terms on node */
	$handler->display->display_options['relationships']['term_node_tid']['id'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['table'] = 'node';
	$handler->display->display_options['relationships']['term_node_tid']['field'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['required'] = 0;
	$handler->display->display_options['relationships']['term_node_tid']['vocabularies'] = array(
	  'cichorieae' => 0,
	  'image_keywords' => 0,
	  'imaging_technique' => 0,
	  'preparation_technique' => 0,
	);
	/* Field: Taxonomy term: Term ID */
	$handler->display->display_options['fields']['tid']['id'] = 'tid';
	$handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
	$handler->display->display_options['fields']['tid']['field'] = 'tid';
	$handler->display->display_options['fields']['tid']['relationship'] = 'term_node_tid';
	$handler->display->display_options['fields']['tid']['label'] = 'id';
	$handler->display->display_options['fields']['tid']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['external'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['html'] = 0;
	$handler->display->display_options['fields']['tid']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['tid']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['tid']['hide_empty'] = 0;
	$handler->display->display_options['fields']['tid']['empty_zero'] = 0;
	$handler->display->display_options['fields']['tid']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['tid']['format_plural'] = 0;
	/* Field: Content: procedures */
	$handler->display->display_options['fields']['field_procedures']['id'] = 'field_procedures';
	$handler->display->display_options['fields']['field_procedures']['table'] = 'field_data_field_procedures';
	$handler->display->display_options['fields']['field_procedures']['field'] = 'field_procedures';
	$handler->display->display_options['fields']['field_procedures']['label'] = 'type';
	$handler->display->display_options['fields']['field_procedures']['alter']['alter_text'] = 1;
	$handler->display->display_options['fields']['field_procedures']['alter']['text'] = 'procedures';
	$handler->display->display_options['fields']['field_procedures']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_procedures']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_procedures']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_procedures']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_procedures']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_procedures']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_procedures']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_procedures']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_procedures']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_procedures']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_procedures']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_procedures']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_procedures']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_procedures']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_procedures']['empty'] = 'procedures';
	$handler->display->display_options['fields']['field_procedures']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_procedures']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_procedures']['hide_alter_empty'] = 0;
	$handler->display->display_options['fields']['field_procedures']['field_api_classes'] = 0;
	/* Field: Content: procedures */
	$handler->display->display_options['fields']['field_procedures_1']['id'] = 'field_procedures_1';
	$handler->display->display_options['fields']['field_procedures_1']['table'] = 'field_data_field_procedures';
	$handler->display->display_options['fields']['field_procedures_1']['field'] = 'field_procedures';
	$handler->display->display_options['fields']['field_procedures_1']['label'] = 'description';
	$handler->display->display_options['fields']['field_procedures_1']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_procedures_1']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_procedures_1']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_procedures_1']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_procedures_1']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_procedures_1']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_procedures_1']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_procedures_1']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_procedures_1']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_procedures_1']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_procedures_1']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_procedures_1']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_procedures_1']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_procedures_1']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_procedures_1']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_procedures_1']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_procedures_1']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_procedures_1']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_procedures_1']['field_api_classes'] = 0;
	/* Sort criterion: Content: Post date */
	$handler->display->display_options['sorts']['created']['id'] = 'created';
	$handler->display->display_options['sorts']['created']['table'] = 'node';
	$handler->display->display_options['sorts']['created']['field'] = 'created';
	$handler->display->display_options['sorts']['created']['order'] = 'DESC';
	/* Filter criterion: Content: Published */
	$handler->display->display_options['filters']['status']['id'] = 'status';
	$handler->display->display_options['filters']['status']['table'] = 'node';
	$handler->display->display_options['filters']['status']['field'] = 'status';
	$handler->display->display_options['filters']['status']['value'] = 1;
	$handler->display->display_options['filters']['status']['group'] = 1;
	$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
	/* Filter criterion: Content: Type */
	$handler->display->display_options['filters']['type']['id'] = 'type';
	$handler->display->display_options['filters']['type']['table'] = 'node';
	$handler->display->display_options['filters']['type']['field'] = 'type';
	$handler->display->display_options['filters']['type']['value'] = array(
	  'spm' => 'spm',
	);

	/* Display: Page */
	$handler = $view->new_display('page', 'Page', 'page');
	$handler->display->display_options['path'] = 'dwca-export/description-procedures/page';

	/* Display: Data export */
	$handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
	$handler->display->display_options['pager']['type'] = 'some';
	$handler->display->display_options['style_plugin'] = 'views_data_export_csv';
	$handler->display->display_options['path'] = 'dwca-export/description-procedures/csv';

	return $view;
}

function _view_description_threats() {

	$view = new view;
	$view->name = 'dwca_export_description_threat';
	$view->description = '';
	$view->tag = 'default';
	$view->base_table = 'node';
	$view->human_name = 'DwC-A Description threats';
	$view->core = 7;
	$view->api_version = '3.0';
	$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

	/* Display: Master */
	$handler = $view->new_display('default', 'Master', 'default');
	$handler->display->display_options['title'] = 'DwC-A Description threats';
	$handler->display->display_options['access']['type'] = 'perm';
	$handler->display->display_options['cache']['type'] = 'none';
	$handler->display->display_options['query']['type'] = 'views_query';
	$handler->display->display_options['query']['options']['query_comment'] = FALSE;
	$handler->display->display_options['exposed_form']['type'] = 'basic';
	$handler->display->display_options['pager']['type'] = 'full';
	$handler->display->display_options['pager']['options']['items_per_page'] = '10';
	$handler->display->display_options['style_plugin'] = 'default';
	$handler->display->display_options['row_plugin'] = 'node';
	/* Relationship: Content: Taxonomy terms on node */
	$handler->display->display_options['relationships']['term_node_tid']['id'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['table'] = 'node';
	$handler->display->display_options['relationships']['term_node_tid']['field'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['required'] = 0;
	$handler->display->display_options['relationships']['term_node_tid']['vocabularies'] = array(
	  'cichorieae' => 0,
	  'image_keywords' => 0,
	  'imaging_technique' => 0,
	  'preparation_technique' => 0,
	);
	/* Field: Taxonomy term: Term ID */
	$handler->display->display_options['fields']['tid']['id'] = 'tid';
	$handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
	$handler->display->display_options['fields']['tid']['field'] = 'tid';
	$handler->display->display_options['fields']['tid']['relationship'] = 'term_node_tid';
	$handler->display->display_options['fields']['tid']['label'] = 'id';
	$handler->display->display_options['fields']['tid']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['external'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['html'] = 0;
	$handler->display->display_options['fields']['tid']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['tid']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['tid']['hide_empty'] = 0;
	$handler->display->display_options['fields']['tid']['empty_zero'] = 0;
	$handler->display->display_options['fields']['tid']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['tid']['format_plural'] = 0;
	/* Field: Content: threats */
	$handler->display->display_options['fields']['field_threats']['id'] = 'field_threats';
	$handler->display->display_options['fields']['field_threats']['table'] = 'field_data_field_threats';
	$handler->display->display_options['fields']['field_threats']['field'] = 'field_threats';
	$handler->display->display_options['fields']['field_threats']['label'] = 'type';
	$handler->display->display_options['fields']['field_threats']['alter']['alter_text'] = 1;
	$handler->display->display_options['fields']['field_threats']['alter']['text'] = 'threats';
	$handler->display->display_options['fields']['field_threats']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_threats']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_threats']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_threats']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_threats']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_threats']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_threats']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_threats']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_threats']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_threats']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_threats']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_threats']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_threats']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_threats']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_threats']['empty'] = 'threats';
	$handler->display->display_options['fields']['field_threats']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_threats']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_threats']['hide_alter_empty'] = 0;
	$handler->display->display_options['fields']['field_threats']['field_api_classes'] = 0;
	/* Field: Content: threats */
	$handler->display->display_options['fields']['field_threats_1']['id'] = 'field_threats_1';
	$handler->display->display_options['fields']['field_threats_1']['table'] = 'field_data_field_threats';
	$handler->display->display_options['fields']['field_threats_1']['field'] = 'field_threats';
	$handler->display->display_options['fields']['field_threats_1']['label'] = 'description';
	$handler->display->display_options['fields']['field_threats_1']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_threats_1']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_threats_1']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_threats_1']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_threats_1']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_threats_1']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_threats_1']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_threats_1']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_threats_1']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_threats_1']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_threats_1']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_threats_1']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_threats_1']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_threats_1']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_threats_1']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_threats_1']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_threats_1']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_threats_1']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_threats_1']['field_api_classes'] = 0;
	/* Sort criterion: Content: Post date */
	$handler->display->display_options['sorts']['created']['id'] = 'created';
	$handler->display->display_options['sorts']['created']['table'] = 'node';
	$handler->display->display_options['sorts']['created']['field'] = 'created';
	$handler->display->display_options['sorts']['created']['order'] = 'DESC';
	/* Filter criterion: Content: Published */
	$handler->display->display_options['filters']['status']['id'] = 'status';
	$handler->display->display_options['filters']['status']['table'] = 'node';
	$handler->display->display_options['filters']['status']['field'] = 'status';
	$handler->display->display_options['filters']['status']['value'] = 1;
	$handler->display->display_options['filters']['status']['group'] = 1;
	$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
	/* Filter criterion: Content: Type */
	$handler->display->display_options['filters']['type']['id'] = 'type';
	$handler->display->display_options['filters']['type']['table'] = 'node';
	$handler->display->display_options['filters']['type']['field'] = 'type';
	$handler->display->display_options['filters']['type']['value'] = array(
	  'spm' => 'spm',
	);

	/* Display: Page */
	$handler = $view->new_display('page', 'Page', 'page');
	$handler->display->display_options['path'] = 'dwca-export/description-threats/page';

	/* Display: Data export */
	$handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
	$handler->display->display_options['pager']['type'] = 'some';
	$handler->display->display_options['style_plugin'] = 'views_data_export_csv';
	$handler->display->display_options['path'] = 'dwca-export/description-threats/csv';

	return $view;
}

function _view_description_associations() {

	$view = new view;
	$view->name = 'dwca_export_description_assoc';
	$view->description = '';
	$view->tag = 'default';
	$view->base_table = 'node';
	$view->human_name = 'DwC-A Description associations';
	$view->core = 7;
	$view->api_version = '3.0';
	$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

	/* Display: Master */
	$handler = $view->new_display('default', 'Master', 'default');
	$handler->display->display_options['title'] = 'DwC-A Description associations';
	$handler->display->display_options['access']['type'] = 'perm';
	$handler->display->display_options['cache']['type'] = 'none';
	$handler->display->display_options['query']['type'] = 'views_query';
	$handler->display->display_options['query']['options']['query_comment'] = FALSE;
	$handler->display->display_options['exposed_form']['type'] = 'basic';
	$handler->display->display_options['pager']['type'] = 'full';
	$handler->display->display_options['pager']['options']['items_per_page'] = '10';
	$handler->display->display_options['style_plugin'] = 'default';
	$handler->display->display_options['row_plugin'] = 'node';
	/* Relationship: Content: Taxonomy terms on node */
	$handler->display->display_options['relationships']['term_node_tid']['id'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['table'] = 'node';
	$handler->display->display_options['relationships']['term_node_tid']['field'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['required'] = 0;
	$handler->display->display_options['relationships']['term_node_tid']['vocabularies'] = array(
	  'cichorieae' => 0,
	  'image_keywords' => 0,
	  'imaging_technique' => 0,
	  'preparation_technique' => 0,
	);
	/* Field: Taxonomy term: Term ID */
	$handler->display->display_options['fields']['tid']['id'] = 'tid';
	$handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
	$handler->display->display_options['fields']['tid']['field'] = 'tid';
	$handler->display->display_options['fields']['tid']['relationship'] = 'term_node_tid';
	$handler->display->display_options['fields']['tid']['label'] = 'id';
	$handler->display->display_options['fields']['tid']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['external'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['html'] = 0;
	$handler->display->display_options['fields']['tid']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['tid']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['tid']['hide_empty'] = 0;
	$handler->display->display_options['fields']['tid']['empty_zero'] = 0;
	$handler->display->display_options['fields']['tid']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['tid']['format_plural'] = 0;
	/* Field: Content: associations */
	$handler->display->display_options['fields']['field_associations']['id'] = 'field_associations';
	$handler->display->display_options['fields']['field_associations']['table'] = 'field_data_field_associations';
	$handler->display->display_options['fields']['field_associations']['field'] = 'field_associations';
	$handler->display->display_options['fields']['field_associations']['label'] = 'type';
	$handler->display->display_options['fields']['field_associations']['alter']['alter_text'] = 1;
	$handler->display->display_options['fields']['field_associations']['alter']['text'] = 'associations';
	$handler->display->display_options['fields']['field_associations']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_associations']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_associations']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_associations']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_associations']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_associations']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_associations']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_associations']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_associations']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_associations']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_associations']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_associations']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_associations']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_associations']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_associations']['empty'] = 'associations';
	$handler->display->display_options['fields']['field_associations']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_associations']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_associations']['hide_alter_empty'] = 0;
	$handler->display->display_options['fields']['field_associations']['field_api_classes'] = 0;
	/* Field: Content: associations */
	$handler->display->display_options['fields']['field_associations_1']['id'] = 'field_associations_1';
	$handler->display->display_options['fields']['field_associations_1']['table'] = 'field_data_field_associations';
	$handler->display->display_options['fields']['field_associations_1']['field'] = 'field_associations';
	$handler->display->display_options['fields']['field_associations_1']['label'] = 'description';
	$handler->display->display_options['fields']['field_associations_1']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_associations_1']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_associations_1']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_associations_1']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_associations_1']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_associations_1']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_associations_1']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_associations_1']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_associations_1']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_associations_1']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_associations_1']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_associations_1']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_associations_1']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_associations_1']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_associations_1']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_associations_1']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_associations_1']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_associations_1']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_associations_1']['field_api_classes'] = 0;
	/* Sort criterion: Content: Post date */
	$handler->display->display_options['sorts']['created']['id'] = 'created';
	$handler->display->display_options['sorts']['created']['table'] = 'node';
	$handler->display->display_options['sorts']['created']['field'] = 'created';
	$handler->display->display_options['sorts']['created']['order'] = 'DESC';
	/* Filter criterion: Content: Published */
	$handler->display->display_options['filters']['status']['id'] = 'status';
	$handler->display->display_options['filters']['status']['table'] = 'node';
	$handler->display->display_options['filters']['status']['field'] = 'status';
	$handler->display->display_options['filters']['status']['value'] = 1;
	$handler->display->display_options['filters']['status']['group'] = 1;
	$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
	/* Filter criterion: Content: Type */
	$handler->display->display_options['filters']['type']['id'] = 'type';
	$handler->display->display_options['filters']['type']['table'] = 'node';
	$handler->display->display_options['filters']['type']['field'] = 'type';
	$handler->display->display_options['filters']['type']['value'] = array(
	  'spm' => 'spm',
	);

	/* Display: Page */
	$handler = $view->new_display('page', 'Page', 'page');
	$handler->display->display_options['path'] = 'dwca-export/description-associations/page';

	/* Display: Data export */
	$handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
	$handler->display->display_options['pager']['type'] = 'some';
	$handler->display->display_options['style_plugin'] = 'views_data_export_csv';
	$handler->display->display_options['path'] = 'dwca-export/description-associations/csv';

	return $view;
}

function _view_description_cyclicity() {

	$view = new view;
	$view->name = 'dwca_export_description_cyclic';
	$view->description = '';
	$view->tag = 'default';
	$view->base_table = 'node';
	$view->human_name = 'DwC-A Description cyclicity';
	$view->core = 7;
	$view->api_version = '3.0';
	$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

	/* Display: Master */
	$handler = $view->new_display('default', 'Master', 'default');
	$handler->display->display_options['title'] = 'DwC-A Description cyclicity';
	$handler->display->display_options['access']['type'] = 'perm';
	$handler->display->display_options['cache']['type'] = 'none';
	$handler->display->display_options['query']['type'] = 'views_query';
	$handler->display->display_options['query']['options']['query_comment'] = FALSE;
	$handler->display->display_options['exposed_form']['type'] = 'basic';
	$handler->display->display_options['pager']['type'] = 'full';
	$handler->display->display_options['pager']['options']['items_per_page'] = '10';
	$handler->display->display_options['style_plugin'] = 'default';
	$handler->display->display_options['row_plugin'] = 'node';
	/* Relationship: Content: Taxonomy terms on node */
	$handler->display->display_options['relationships']['term_node_tid']['id'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['table'] = 'node';
	$handler->display->display_options['relationships']['term_node_tid']['field'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['required'] = 0;
	$handler->display->display_options['relationships']['term_node_tid']['vocabularies'] = array(
	  'cichorieae' => 0,
	  'image_keywords' => 0,
	  'imaging_technique' => 0,
	  'preparation_technique' => 0,
	);
	/* Field: Taxonomy term: Term ID */
	$handler->display->display_options['fields']['tid']['id'] = 'tid';
	$handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
	$handler->display->display_options['fields']['tid']['field'] = 'tid';
	$handler->display->display_options['fields']['tid']['relationship'] = 'term_node_tid';
	$handler->display->display_options['fields']['tid']['label'] = 'id';
	$handler->display->display_options['fields']['tid']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['external'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['html'] = 0;
	$handler->display->display_options['fields']['tid']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['tid']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['tid']['hide_empty'] = 0;
	$handler->display->display_options['fields']['tid']['empty_zero'] = 0;
	$handler->display->display_options['fields']['tid']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['tid']['format_plural'] = 0;
	/* Field: Content: cyclicity */
	$handler->display->display_options['fields']['field_cyclicity']['id'] = 'field_cyclicity';
	$handler->display->display_options['fields']['field_cyclicity']['table'] = 'field_data_field_cyclicity';
	$handler->display->display_options['fields']['field_cyclicity']['field'] = 'field_cyclicity';
	$handler->display->display_options['fields']['field_cyclicity']['label'] = 'type';
	$handler->display->display_options['fields']['field_cyclicity']['alter']['alter_text'] = 1;
	$handler->display->display_options['fields']['field_cyclicity']['alter']['text'] = 'cyclicity';
	$handler->display->display_options['fields']['field_cyclicity']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_cyclicity']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_cyclicity']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_cyclicity']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_cyclicity']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_cyclicity']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_cyclicity']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_cyclicity']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_cyclicity']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_cyclicity']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_cyclicity']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_cyclicity']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_cyclicity']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_cyclicity']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_cyclicity']['empty'] = 'cyclicity';
	$handler->display->display_options['fields']['field_cyclicity']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_cyclicity']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_cyclicity']['hide_alter_empty'] = 0;
	$handler->display->display_options['fields']['field_cyclicity']['field_api_classes'] = 0;
	/* Field: Content: cyclicity */
	$handler->display->display_options['fields']['field_cyclicity_1']['id'] = 'field_cyclicity_1';
	$handler->display->display_options['fields']['field_cyclicity_1']['table'] = 'field_data_field_cyclicity';
	$handler->display->display_options['fields']['field_cyclicity_1']['field'] = 'field_cyclicity';
	$handler->display->display_options['fields']['field_cyclicity_1']['label'] = 'description';
	$handler->display->display_options['fields']['field_cyclicity_1']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_cyclicity_1']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_cyclicity_1']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_cyclicity_1']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_cyclicity_1']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_cyclicity_1']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_cyclicity_1']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_cyclicity_1']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_cyclicity_1']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_cyclicity_1']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_cyclicity_1']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_cyclicity_1']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_cyclicity_1']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_cyclicity_1']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_cyclicity_1']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_cyclicity_1']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_cyclicity_1']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_cyclicity_1']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_cyclicity_1']['field_api_classes'] = 0;
	/* Sort criterion: Content: Post date */
	$handler->display->display_options['sorts']['created']['id'] = 'created';
	$handler->display->display_options['sorts']['created']['table'] = 'node';
	$handler->display->display_options['sorts']['created']['field'] = 'created';
	$handler->display->display_options['sorts']['created']['order'] = 'DESC';
	/* Filter criterion: Content: Published */
	$handler->display->display_options['filters']['status']['id'] = 'status';
	$handler->display->display_options['filters']['status']['table'] = 'node';
	$handler->display->display_options['filters']['status']['field'] = 'status';
	$handler->display->display_options['filters']['status']['value'] = 1;
	$handler->display->display_options['filters']['status']['group'] = 1;
	$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
	/* Filter criterion: Content: Type */
	$handler->display->display_options['filters']['type']['id'] = 'type';
	$handler->display->display_options['filters']['type']['table'] = 'node';
	$handler->display->display_options['filters']['type']['field'] = 'type';
	$handler->display->display_options['filters']['type']['value'] = array(
	  'spm' => 'spm',
	);

	/* Display: Page */
	$handler = $view->new_display('page', 'Page', 'page');
	$handler->display->display_options['path'] = 'dwca-export/description-cyclicity/page';

	/* Display: Data export */
	$handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
	$handler->display->display_options['pager']['type'] = 'some';
	$handler->display->display_options['style_plugin'] = 'views_data_export_csv';
	$handler->display->display_options['path'] = 'dwca-export/description-cyclicity/csv';

	return $view;
}

function _view_description_diagnostic_description() {

	$view = new view;
	$view->name = 'dwca_export_description_diag';
	$view->description = '';
	$view->tag = 'default';
	$view->base_table = 'node';
	$view->human_name = 'DwC-A Description diagnostic_description';
	$view->core = 7;
	$view->api_version = '3.0';
	$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

	/* Display: Master */
	$handler = $view->new_display('default', 'Master', 'default');
	$handler->display->display_options['title'] = 'DwC-A Description diagnostic_description';
	$handler->display->display_options['access']['type'] = 'perm';
	$handler->display->display_options['cache']['type'] = 'none';
	$handler->display->display_options['query']['type'] = 'views_query';
	$handler->display->display_options['query']['options']['query_comment'] = FALSE;
	$handler->display->display_options['exposed_form']['type'] = 'basic';
	$handler->display->display_options['pager']['type'] = 'full';
	$handler->display->display_options['pager']['options']['items_per_page'] = '10';
	$handler->display->display_options['style_plugin'] = 'default';
	$handler->display->display_options['row_plugin'] = 'node';
	/* Relationship: Content: Taxonomy terms on node */
	$handler->display->display_options['relationships']['term_node_tid']['id'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['table'] = 'node';
	$handler->display->display_options['relationships']['term_node_tid']['field'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['required'] = 0;
	$handler->display->display_options['relationships']['term_node_tid']['vocabularies'] = array(
	  'cichorieae' => 0,
	  'image_keywords' => 0,
	  'imaging_technique' => 0,
	  'preparation_technique' => 0,
	);
	/* Field: Taxonomy term: Term ID */
	$handler->display->display_options['fields']['tid']['id'] = 'tid';
	$handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
	$handler->display->display_options['fields']['tid']['field'] = 'tid';
	$handler->display->display_options['fields']['tid']['relationship'] = 'term_node_tid';
	$handler->display->display_options['fields']['tid']['label'] = 'id';
	$handler->display->display_options['fields']['tid']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['external'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['html'] = 0;
	$handler->display->display_options['fields']['tid']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['tid']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['tid']['hide_empty'] = 0;
	$handler->display->display_options['fields']['tid']['empty_zero'] = 0;
	$handler->display->display_options['fields']['tid']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['tid']['format_plural'] = 0;
	/* Field: Content: diagnostic_description */
	$handler->display->display_options['fields']['field_diagnostic_description']['id'] = 'field_diagnostic_description';
	$handler->display->display_options['fields']['field_diagnostic_description']['table'] = 'field_data_field_diagnostic_description';
	$handler->display->display_options['fields']['field_diagnostic_description']['field'] = 'field_diagnostic_description';
	$handler->display->display_options['fields']['field_diagnostic_description']['label'] = 'type';
	$handler->display->display_options['fields']['field_diagnostic_description']['alter']['alter_text'] = 1;
	$handler->display->display_options['fields']['field_diagnostic_description']['alter']['text'] = 'diagnostic_description';
	$handler->display->display_options['fields']['field_diagnostic_description']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_diagnostic_description']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_diagnostic_description']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_diagnostic_description']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_diagnostic_description']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_diagnostic_description']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_diagnostic_description']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_diagnostic_description']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_diagnostic_description']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_diagnostic_description']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_diagnostic_description']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_diagnostic_description']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_diagnostic_description']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_diagnostic_description']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_diagnostic_description']['empty'] = 'diagnostic_description';
	$handler->display->display_options['fields']['field_diagnostic_description']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_diagnostic_description']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_diagnostic_description']['hide_alter_empty'] = 0;
	$handler->display->display_options['fields']['field_diagnostic_description']['field_api_classes'] = 0;
	/* Field: Content: diagnostic_description */
	$handler->display->display_options['fields']['field_diagnostic_description_1']['id'] = 'field_diagnostic_description_1';
	$handler->display->display_options['fields']['field_diagnostic_description_1']['table'] = 'field_data_field_diagnostic_description';
	$handler->display->display_options['fields']['field_diagnostic_description_1']['field'] = 'field_diagnostic_description';
	$handler->display->display_options['fields']['field_diagnostic_description_1']['label'] = 'description';
	$handler->display->display_options['fields']['field_diagnostic_description_1']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_diagnostic_description_1']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_diagnostic_description_1']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_diagnostic_description_1']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_diagnostic_description_1']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_diagnostic_description_1']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_diagnostic_description_1']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_diagnostic_description_1']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_diagnostic_description_1']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_diagnostic_description_1']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_diagnostic_description_1']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_diagnostic_description_1']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_diagnostic_description_1']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_diagnostic_description_1']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_diagnostic_description_1']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_diagnostic_description_1']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_diagnostic_description_1']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_diagnostic_description_1']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_diagnostic_description_1']['field_api_classes'] = 0;
	/* Sort criterion: Content: Post date */
	$handler->display->display_options['sorts']['created']['id'] = 'created';
	$handler->display->display_options['sorts']['created']['table'] = 'node';
	$handler->display->display_options['sorts']['created']['field'] = 'created';
	$handler->display->display_options['sorts']['created']['order'] = 'DESC';
	/* Filter criterion: Content: Published */
	$handler->display->display_options['filters']['status']['id'] = 'status';
	$handler->display->display_options['filters']['status']['table'] = 'node';
	$handler->display->display_options['filters']['status']['field'] = 'status';
	$handler->display->display_options['filters']['status']['value'] = 1;
	$handler->display->display_options['filters']['status']['group'] = 1;
	$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
	/* Filter criterion: Content: Type */
	$handler->display->display_options['filters']['type']['id'] = 'type';
	$handler->display->display_options['filters']['type']['table'] = 'node';
	$handler->display->display_options['filters']['type']['field'] = 'type';
	$handler->display->display_options['filters']['type']['value'] = array(
	  'spm' => 'spm',
	);

	/* Display: Page */
	$handler = $view->new_display('page', 'Page', 'page');
	$handler->display->display_options['path'] = 'dwca-export/description-diagnostic_description/page';

	/* Display: Data export */
	$handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
	$handler->display->display_options['pager']['type'] = 'some';
	$handler->display->display_options['style_plugin'] = 'views_data_export_csv';
	$handler->display->display_options['path'] = 'dwca-export/description-diagnostic_description/csv';

	return $view;
}

function _view_description_diseases() {

	$view = new view;
	$view->name = 'dwca_export_description_disease';
	$view->description = '';
	$view->tag = 'default';
	$view->base_table = 'node';
	$view->human_name = 'DwC-A Description diseases';
	$view->core = 7;
	$view->api_version = '3.0';
	$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

	/* Display: Master */
	$handler = $view->new_display('default', 'Master', 'default');
	$handler->display->display_options['title'] = 'DwC-A Description diseases';
	$handler->display->display_options['access']['type'] = 'perm';
	$handler->display->display_options['cache']['type'] = 'none';
	$handler->display->display_options['query']['type'] = 'views_query';
	$handler->display->display_options['query']['options']['query_comment'] = FALSE;
	$handler->display->display_options['exposed_form']['type'] = 'basic';
	$handler->display->display_options['pager']['type'] = 'full';
	$handler->display->display_options['pager']['options']['items_per_page'] = '10';
	$handler->display->display_options['style_plugin'] = 'default';
	$handler->display->display_options['row_plugin'] = 'node';
	/* Relationship: Content: Taxonomy terms on node */
	$handler->display->display_options['relationships']['term_node_tid']['id'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['table'] = 'node';
	$handler->display->display_options['relationships']['term_node_tid']['field'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['required'] = 0;
	$handler->display->display_options['relationships']['term_node_tid']['vocabularies'] = array(
	  'cichorieae' => 0,
	  'image_keywords' => 0,
	  'imaging_technique' => 0,
	  'preparation_technique' => 0,
	);
	/* Field: Taxonomy term: Term ID */
	$handler->display->display_options['fields']['tid']['id'] = 'tid';
	$handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
	$handler->display->display_options['fields']['tid']['field'] = 'tid';
	$handler->display->display_options['fields']['tid']['relationship'] = 'term_node_tid';
	$handler->display->display_options['fields']['tid']['label'] = 'id';
	$handler->display->display_options['fields']['tid']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['external'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['html'] = 0;
	$handler->display->display_options['fields']['tid']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['tid']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['tid']['hide_empty'] = 0;
	$handler->display->display_options['fields']['tid']['empty_zero'] = 0;
	$handler->display->display_options['fields']['tid']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['tid']['format_plural'] = 0;
	/* Field: Content: diseases */
	$handler->display->display_options['fields']['field_diseases']['id'] = 'field_diseases';
	$handler->display->display_options['fields']['field_diseases']['table'] = 'field_data_field_diseases';
	$handler->display->display_options['fields']['field_diseases']['field'] = 'field_diseases';
	$handler->display->display_options['fields']['field_diseases']['label'] = 'type';
	$handler->display->display_options['fields']['field_diseases']['alter']['alter_text'] = 1;
	$handler->display->display_options['fields']['field_diseases']['alter']['text'] = 'diseases';
	$handler->display->display_options['fields']['field_diseases']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_diseases']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_diseases']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_diseases']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_diseases']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_diseases']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_diseases']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_diseases']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_diseases']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_diseases']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_diseases']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_diseases']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_diseases']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_diseases']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_diseases']['empty'] = 'diseases';
	$handler->display->display_options['fields']['field_diseases']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_diseases']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_diseases']['hide_alter_empty'] = 0;
	$handler->display->display_options['fields']['field_diseases']['field_api_classes'] = 0;
	/* Field: Content: diseases */
	$handler->display->display_options['fields']['field_diseases_1']['id'] = 'field_diseases_1';
	$handler->display->display_options['fields']['field_diseases_1']['table'] = 'field_data_field_diseases';
	$handler->display->display_options['fields']['field_diseases_1']['field'] = 'field_diseases';
	$handler->display->display_options['fields']['field_diseases_1']['label'] = 'description';
	$handler->display->display_options['fields']['field_diseases_1']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_diseases_1']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_diseases_1']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_diseases_1']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_diseases_1']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_diseases_1']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_diseases_1']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_diseases_1']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_diseases_1']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_diseases_1']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_diseases_1']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_diseases_1']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_diseases_1']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_diseases_1']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_diseases_1']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_diseases_1']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_diseases_1']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_diseases_1']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_diseases_1']['field_api_classes'] = 0;
	/* Sort criterion: Content: Post date */
	$handler->display->display_options['sorts']['created']['id'] = 'created';
	$handler->display->display_options['sorts']['created']['table'] = 'node';
	$handler->display->display_options['sorts']['created']['field'] = 'created';
	$handler->display->display_options['sorts']['created']['order'] = 'DESC';
	/* Filter criterion: Content: Published */
	$handler->display->display_options['filters']['status']['id'] = 'status';
	$handler->display->display_options['filters']['status']['table'] = 'node';
	$handler->display->display_options['filters']['status']['field'] = 'status';
	$handler->display->display_options['filters']['status']['value'] = 1;
	$handler->display->display_options['filters']['status']['group'] = 1;
	$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
	/* Filter criterion: Content: Type */
	$handler->display->display_options['filters']['type']['id'] = 'type';
	$handler->display->display_options['filters']['type']['table'] = 'node';
	$handler->display->display_options['filters']['type']['field'] = 'type';
	$handler->display->display_options['filters']['type']['value'] = array(
	  'spm' => 'spm',
	);

	/* Display: Page */
	$handler = $view->new_display('page', 'Page', 'page');
	$handler->display->display_options['path'] = 'dwca-export/description-diseases/page';

	/* Display: Data export */
	$handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
	$handler->display->display_options['pager']['type'] = 'some';
	$handler->display->display_options['style_plugin'] = 'views_data_export_csv';
	$handler->display->display_options['path'] = 'dwca-export/description-diseases/csv';

	return $view;
}

function _view_description_dispersal() {

	$view = new view;
	$view->name = 'dwca_export_description_disperse';
	$view->description = '';
	$view->tag = 'default';
	$view->base_table = 'node';
	$view->human_name = 'DwC-A Description dispersal';
	$view->core = 7;
	$view->api_version = '3.0';
	$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

	/* Display: Master */
	$handler = $view->new_display('default', 'Master', 'default');
	$handler->display->display_options['title'] = 'DwC-A Description dispersal';
	$handler->display->display_options['access']['type'] = 'perm';
	$handler->display->display_options['cache']['type'] = 'none';
	$handler->display->display_options['query']['type'] = 'views_query';
	$handler->display->display_options['query']['options']['query_comment'] = FALSE;
	$handler->display->display_options['exposed_form']['type'] = 'basic';
	$handler->display->display_options['pager']['type'] = 'full';
	$handler->display->display_options['pager']['options']['items_per_page'] = '10';
	$handler->display->display_options['style_plugin'] = 'default';
	$handler->display->display_options['row_plugin'] = 'node';
	/* Relationship: Content: Taxonomy terms on node */
	$handler->display->display_options['relationships']['term_node_tid']['id'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['table'] = 'node';
	$handler->display->display_options['relationships']['term_node_tid']['field'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['required'] = 0;
	$handler->display->display_options['relationships']['term_node_tid']['vocabularies'] = array(
	  'cichorieae' => 0,
	  'image_keywords' => 0,
	  'imaging_technique' => 0,
	  'preparation_technique' => 0,
	);
	/* Field: Taxonomy term: Term ID */
	$handler->display->display_options['fields']['tid']['id'] = 'tid';
	$handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
	$handler->display->display_options['fields']['tid']['field'] = 'tid';
	$handler->display->display_options['fields']['tid']['relationship'] = 'term_node_tid';
	$handler->display->display_options['fields']['tid']['label'] = 'id';
	$handler->display->display_options['fields']['tid']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['external'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['html'] = 0;
	$handler->display->display_options['fields']['tid']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['tid']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['tid']['hide_empty'] = 0;
	$handler->display->display_options['fields']['tid']['empty_zero'] = 0;
	$handler->display->display_options['fields']['tid']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['tid']['format_plural'] = 0;
	/* Field: Content: dispersal */
	$handler->display->display_options['fields']['field_dispersal']['id'] = 'field_dispersal';
	$handler->display->display_options['fields']['field_dispersal']['table'] = 'field_data_field_dispersal';
	$handler->display->display_options['fields']['field_dispersal']['field'] = 'field_dispersal';
	$handler->display->display_options['fields']['field_dispersal']['label'] = 'type';
	$handler->display->display_options['fields']['field_dispersal']['alter']['alter_text'] = 1;
	$handler->display->display_options['fields']['field_dispersal']['alter']['text'] = 'dispersal';
	$handler->display->display_options['fields']['field_dispersal']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_dispersal']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_dispersal']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_dispersal']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_dispersal']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_dispersal']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_dispersal']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_dispersal']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_dispersal']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_dispersal']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_dispersal']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_dispersal']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_dispersal']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_dispersal']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_dispersal']['empty'] = 'dispersal';
	$handler->display->display_options['fields']['field_dispersal']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_dispersal']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_dispersal']['hide_alter_empty'] = 0;
	$handler->display->display_options['fields']['field_dispersal']['field_api_classes'] = 0;
	/* Field: Content: dispersal */
	$handler->display->display_options['fields']['field_dispersal_1']['id'] = 'field_dispersal_1';
	$handler->display->display_options['fields']['field_dispersal_1']['table'] = 'field_data_field_dispersal';
	$handler->display->display_options['fields']['field_dispersal_1']['field'] = 'field_dispersal';
	$handler->display->display_options['fields']['field_dispersal_1']['label'] = 'description';
	$handler->display->display_options['fields']['field_dispersal_1']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_dispersal_1']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_dispersal_1']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_dispersal_1']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_dispersal_1']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_dispersal_1']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_dispersal_1']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_dispersal_1']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_dispersal_1']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_dispersal_1']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_dispersal_1']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_dispersal_1']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_dispersal_1']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_dispersal_1']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_dispersal_1']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_dispersal_1']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_dispersal_1']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_dispersal_1']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_dispersal_1']['field_api_classes'] = 0;
	/* Sort criterion: Content: Post date */
	$handler->display->display_options['sorts']['created']['id'] = 'created';
	$handler->display->display_options['sorts']['created']['table'] = 'node';
	$handler->display->display_options['sorts']['created']['field'] = 'created';
	$handler->display->display_options['sorts']['created']['order'] = 'DESC';
	/* Filter criterion: Content: Published */
	$handler->display->display_options['filters']['status']['id'] = 'status';
	$handler->display->display_options['filters']['status']['table'] = 'node';
	$handler->display->display_options['filters']['status']['field'] = 'status';
	$handler->display->display_options['filters']['status']['value'] = 1;
	$handler->display->display_options['filters']['status']['group'] = 1;
	$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
	/* Filter criterion: Content: Type */
	$handler->display->display_options['filters']['type']['id'] = 'type';
	$handler->display->display_options['filters']['type']['table'] = 'node';
	$handler->display->display_options['filters']['type']['field'] = 'type';
	$handler->display->display_options['filters']['type']['value'] = array(
	  'spm' => 'spm',
	);

	/* Display: Page */
	$handler = $view->new_display('page', 'Page', 'page');
	$handler->display->display_options['path'] = 'dwca-export/description-dispersal/page';

	/* Display: Data export */
	$handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
	$handler->display->display_options['pager']['type'] = 'some';
	$handler->display->display_options['style_plugin'] = 'views_data_export_csv';
	$handler->display->display_options['path'] = 'dwca-export/description-dispersal/csv';

	return $view;
}

function _view_description_distribution() {

	$view = new view;
	$view->name = 'dwca_export_description_dist';
	$view->description = '';
	$view->tag = 'default';
	$view->base_table = 'node';
	$view->human_name = 'DwC-A Description distribution';
	$view->core = 7;
	$view->api_version = '3.0';
	$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

	/* Display: Master */
	$handler = $view->new_display('default', 'Master', 'default');
	$handler->display->display_options['title'] = 'DwC-A Description distribution';
	$handler->display->display_options['access']['type'] = 'perm';
	$handler->display->display_options['cache']['type'] = 'none';
	$handler->display->display_options['query']['type'] = 'views_query';
	$handler->display->display_options['query']['options']['query_comment'] = FALSE;
	$handler->display->display_options['exposed_form']['type'] = 'basic';
	$handler->display->display_options['pager']['type'] = 'full';
	$handler->display->display_options['pager']['options']['items_per_page'] = '10';
	$handler->display->display_options['style_plugin'] = 'default';
	$handler->display->display_options['row_plugin'] = 'node';
	/* Relationship: Content: Taxonomy terms on node */
	$handler->display->display_options['relationships']['term_node_tid']['id'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['table'] = 'node';
	$handler->display->display_options['relationships']['term_node_tid']['field'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['required'] = 0;
	$handler->display->display_options['relationships']['term_node_tid']['vocabularies'] = array(
	  'cichorieae' => 0,
	  'image_keywords' => 0,
	  'imaging_technique' => 0,
	  'preparation_technique' => 0,
	);
	/* Field: Taxonomy term: Term ID */
	$handler->display->display_options['fields']['tid']['id'] = 'tid';
	$handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
	$handler->display->display_options['fields']['tid']['field'] = 'tid';
	$handler->display->display_options['fields']['tid']['relationship'] = 'term_node_tid';
	$handler->display->display_options['fields']['tid']['label'] = 'id';
	$handler->display->display_options['fields']['tid']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['external'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['html'] = 0;
	$handler->display->display_options['fields']['tid']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['tid']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['tid']['hide_empty'] = 0;
	$handler->display->display_options['fields']['tid']['empty_zero'] = 0;
	$handler->display->display_options['fields']['tid']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['tid']['format_plural'] = 0;
	/* Field: Content: distribution */
	$handler->display->display_options['fields']['field_distribution']['id'] = 'field_distribution';
	$handler->display->display_options['fields']['field_distribution']['table'] = 'field_data_field_distribution';
	$handler->display->display_options['fields']['field_distribution']['field'] = 'field_distribution';
	$handler->display->display_options['fields']['field_distribution']['label'] = 'type';
	$handler->display->display_options['fields']['field_distribution']['alter']['alter_text'] = 1;
	$handler->display->display_options['fields']['field_distribution']['alter']['text'] = 'distribution';
	$handler->display->display_options['fields']['field_distribution']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_distribution']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_distribution']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_distribution']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_distribution']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_distribution']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_distribution']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_distribution']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_distribution']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_distribution']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_distribution']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_distribution']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_distribution']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_distribution']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_distribution']['empty'] = 'distribution';
	$handler->display->display_options['fields']['field_distribution']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_distribution']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_distribution']['hide_alter_empty'] = 0;
	$handler->display->display_options['fields']['field_distribution']['field_api_classes'] = 0;
	/* Field: Content: distribution */
	$handler->display->display_options['fields']['field_distribution_1']['id'] = 'field_distribution_1';
	$handler->display->display_options['fields']['field_distribution_1']['table'] = 'field_data_field_distribution';
	$handler->display->display_options['fields']['field_distribution_1']['field'] = 'field_distribution';
	$handler->display->display_options['fields']['field_distribution_1']['label'] = 'description';
	$handler->display->display_options['fields']['field_distribution_1']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_distribution_1']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_distribution_1']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_distribution_1']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_distribution_1']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_distribution_1']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_distribution_1']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_distribution_1']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_distribution_1']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_distribution_1']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_distribution_1']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_distribution_1']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_distribution_1']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_distribution_1']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_distribution_1']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_distribution_1']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_distribution_1']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_distribution_1']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_distribution_1']['field_api_classes'] = 0;
	/* Sort criterion: Content: Post date */
	$handler->display->display_options['sorts']['created']['id'] = 'created';
	$handler->display->display_options['sorts']['created']['table'] = 'node';
	$handler->display->display_options['sorts']['created']['field'] = 'created';
	$handler->display->display_options['sorts']['created']['order'] = 'DESC';
	/* Filter criterion: Content: Published */
	$handler->display->display_options['filters']['status']['id'] = 'status';
	$handler->display->display_options['filters']['status']['table'] = 'node';
	$handler->display->display_options['filters']['status']['field'] = 'status';
	$handler->display->display_options['filters']['status']['value'] = 1;
	$handler->display->display_options['filters']['status']['group'] = 1;
	$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
	/* Filter criterion: Content: Type */
	$handler->display->display_options['filters']['type']['id'] = 'type';
	$handler->display->display_options['filters']['type']['table'] = 'node';
	$handler->display->display_options['filters']['type']['field'] = 'type';
	$handler->display->display_options['filters']['type']['value'] = array(
	  'spm' => 'spm',
	);

	/* Display: Page */
	$handler = $view->new_display('page', 'Page', 'page');
	$handler->display->display_options['path'] = 'dwca-export/description-distribution/page';

	/* Display: Data export */
	$handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
	$handler->display->display_options['pager']['type'] = 'some';
	$handler->display->display_options['style_plugin'] = 'views_data_export_csv';
	$handler->display->display_options['path'] = 'dwca-export/description-distribution/csv';

	return $view;
}

function _view_description_ecology() {

	$view = new view;
	$view->name = 'dwca_export_description_ecol';
	$view->description = '';
	$view->tag = 'default';
	$view->base_table = 'node';
	$view->human_name = 'DwC-A Description ecology';
	$view->core = 7;
	$view->api_version = '3.0';
	$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

	/* Display: Master */
	$handler = $view->new_display('default', 'Master', 'default');
	$handler->display->display_options['title'] = 'DwC-A Description ecology';
	$handler->display->display_options['access']['type'] = 'perm';
	$handler->display->display_options['cache']['type'] = 'none';
	$handler->display->display_options['query']['type'] = 'views_query';
	$handler->display->display_options['query']['options']['query_comment'] = FALSE;
	$handler->display->display_options['exposed_form']['type'] = 'basic';
	$handler->display->display_options['pager']['type'] = 'full';
	$handler->display->display_options['pager']['options']['items_per_page'] = '10';
	$handler->display->display_options['style_plugin'] = 'default';
	$handler->display->display_options['row_plugin'] = 'node';
	/* Relationship: Content: Taxonomy terms on node */
	$handler->display->display_options['relationships']['term_node_tid']['id'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['table'] = 'node';
	$handler->display->display_options['relationships']['term_node_tid']['field'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['required'] = 0;
	$handler->display->display_options['relationships']['term_node_tid']['vocabularies'] = array(
	  'cichorieae' => 0,
	  'image_keywords' => 0,
	  'imaging_technique' => 0,
	  'preparation_technique' => 0,
	);
	/* Field: Taxonomy term: Term ID */
	$handler->display->display_options['fields']['tid']['id'] = 'tid';
	$handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
	$handler->display->display_options['fields']['tid']['field'] = 'tid';
	$handler->display->display_options['fields']['tid']['relationship'] = 'term_node_tid';
	$handler->display->display_options['fields']['tid']['label'] = 'id';
	$handler->display->display_options['fields']['tid']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['external'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['html'] = 0;
	$handler->display->display_options['fields']['tid']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['tid']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['tid']['hide_empty'] = 0;
	$handler->display->display_options['fields']['tid']['empty_zero'] = 0;
	$handler->display->display_options['fields']['tid']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['tid']['format_plural'] = 0;
	/* Field: Content: ecology */
	$handler->display->display_options['fields']['field_ecology']['id'] = 'field_ecology';
	$handler->display->display_options['fields']['field_ecology']['table'] = 'field_data_field_ecology';
	$handler->display->display_options['fields']['field_ecology']['field'] = 'field_ecology';
	$handler->display->display_options['fields']['field_ecology']['label'] = 'type';
	$handler->display->display_options['fields']['field_ecology']['alter']['alter_text'] = 1;
	$handler->display->display_options['fields']['field_ecology']['alter']['text'] = 'ecology';
	$handler->display->display_options['fields']['field_ecology']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_ecology']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_ecology']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_ecology']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_ecology']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_ecology']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_ecology']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_ecology']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_ecology']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_ecology']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_ecology']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_ecology']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_ecology']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_ecology']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_ecology']['empty'] = 'ecology';
	$handler->display->display_options['fields']['field_ecology']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_ecology']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_ecology']['hide_alter_empty'] = 0;
	$handler->display->display_options['fields']['field_ecology']['field_api_classes'] = 0;
	/* Field: Content: ecology */
	$handler->display->display_options['fields']['field_ecology_1']['id'] = 'field_ecology_1';
	$handler->display->display_options['fields']['field_ecology_1']['table'] = 'field_data_field_ecology';
	$handler->display->display_options['fields']['field_ecology_1']['field'] = 'field_ecology';
	$handler->display->display_options['fields']['field_ecology_1']['label'] = 'description';
	$handler->display->display_options['fields']['field_ecology_1']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_ecology_1']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_ecology_1']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_ecology_1']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_ecology_1']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_ecology_1']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_ecology_1']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_ecology_1']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_ecology_1']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_ecology_1']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_ecology_1']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_ecology_1']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_ecology_1']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_ecology_1']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_ecology_1']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_ecology_1']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_ecology_1']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_ecology_1']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_ecology_1']['field_api_classes'] = 0;
	/* Sort criterion: Content: Post date */
	$handler->display->display_options['sorts']['created']['id'] = 'created';
	$handler->display->display_options['sorts']['created']['table'] = 'node';
	$handler->display->display_options['sorts']['created']['field'] = 'created';
	$handler->display->display_options['sorts']['created']['order'] = 'DESC';
	/* Filter criterion: Content: Published */
	$handler->display->display_options['filters']['status']['id'] = 'status';
	$handler->display->display_options['filters']['status']['table'] = 'node';
	$handler->display->display_options['filters']['status']['field'] = 'status';
	$handler->display->display_options['filters']['status']['value'] = 1;
	$handler->display->display_options['filters']['status']['group'] = 1;
	$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
	/* Filter criterion: Content: Type */
	$handler->display->display_options['filters']['type']['id'] = 'type';
	$handler->display->display_options['filters']['type']['table'] = 'node';
	$handler->display->display_options['filters']['type']['field'] = 'type';
	$handler->display->display_options['filters']['type']['value'] = array(
	  'spm' => 'spm',
	);

	/* Display: Page */
	$handler = $view->new_display('page', 'Page', 'page');
	$handler->display->display_options['path'] = 'dwca-export/description-ecology/page';

	/* Display: Data export */
	$handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
	$handler->display->display_options['pager']['type'] = 'some';
	$handler->display->display_options['style_plugin'] = 'views_data_export_csv';
	$handler->display->display_options['path'] = 'dwca-export/description-ecology/csv';

	return $view;
}

function _view_description_evolution() {

	$view = new view;
	$view->name = 'dwca_export_description_evolve';
	$view->description = '';
	$view->tag = 'default';
	$view->base_table = 'node';
	$view->human_name = 'DwC-A Description evolution';
	$view->core = 7;
	$view->api_version = '3.0';
	$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

	/* Display: Master */
	$handler = $view->new_display('default', 'Master', 'default');
	$handler->display->display_options['title'] = 'DwC-A Description evolution';
	$handler->display->display_options['access']['type'] = 'perm';
	$handler->display->display_options['cache']['type'] = 'none';
	$handler->display->display_options['query']['type'] = 'views_query';
	$handler->display->display_options['query']['options']['query_comment'] = FALSE;
	$handler->display->display_options['exposed_form']['type'] = 'basic';
	$handler->display->display_options['pager']['type'] = 'full';
	$handler->display->display_options['pager']['options']['items_per_page'] = '10';
	$handler->display->display_options['style_plugin'] = 'default';
	$handler->display->display_options['row_plugin'] = 'node';
	/* Relationship: Content: Taxonomy terms on node */
	$handler->display->display_options['relationships']['term_node_tid']['id'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['table'] = 'node';
	$handler->display->display_options['relationships']['term_node_tid']['field'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['required'] = 0;
	$handler->display->display_options['relationships']['term_node_tid']['vocabularies'] = array(
	  'cichorieae' => 0,
	  'image_keywords' => 0,
	  'imaging_technique' => 0,
	  'preparation_technique' => 0,
	);
	/* Field: Taxonomy term: Term ID */
	$handler->display->display_options['fields']['tid']['id'] = 'tid';
	$handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
	$handler->display->display_options['fields']['tid']['field'] = 'tid';
	$handler->display->display_options['fields']['tid']['relationship'] = 'term_node_tid';
	$handler->display->display_options['fields']['tid']['label'] = 'id';
	$handler->display->display_options['fields']['tid']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['external'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['html'] = 0;
	$handler->display->display_options['fields']['tid']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['tid']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['tid']['hide_empty'] = 0;
	$handler->display->display_options['fields']['tid']['empty_zero'] = 0;
	$handler->display->display_options['fields']['tid']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['tid']['format_plural'] = 0;
	/* Field: Content: evolution */
	$handler->display->display_options['fields']['field_evolution']['id'] = 'field_evolution';
	$handler->display->display_options['fields']['field_evolution']['table'] = 'field_data_field_evolution';
	$handler->display->display_options['fields']['field_evolution']['field'] = 'field_evolution';
	$handler->display->display_options['fields']['field_evolution']['label'] = 'type';
	$handler->display->display_options['fields']['field_evolution']['alter']['alter_text'] = 1;
	$handler->display->display_options['fields']['field_evolution']['alter']['text'] = 'evolution';
	$handler->display->display_options['fields']['field_evolution']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_evolution']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_evolution']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_evolution']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_evolution']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_evolution']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_evolution']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_evolution']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_evolution']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_evolution']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_evolution']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_evolution']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_evolution']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_evolution']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_evolution']['empty'] = 'evolution';
	$handler->display->display_options['fields']['field_evolution']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_evolution']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_evolution']['hide_alter_empty'] = 0;
	$handler->display->display_options['fields']['field_evolution']['field_api_classes'] = 0;
	/* Field: Content: evolution */
	$handler->display->display_options['fields']['field_evolution_1']['id'] = 'field_evolution_1';
	$handler->display->display_options['fields']['field_evolution_1']['table'] = 'field_data_field_evolution';
	$handler->display->display_options['fields']['field_evolution_1']['field'] = 'field_evolution';
	$handler->display->display_options['fields']['field_evolution_1']['label'] = 'description';
	$handler->display->display_options['fields']['field_evolution_1']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_evolution_1']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_evolution_1']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_evolution_1']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_evolution_1']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_evolution_1']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_evolution_1']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_evolution_1']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_evolution_1']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_evolution_1']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_evolution_1']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_evolution_1']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_evolution_1']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_evolution_1']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_evolution_1']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_evolution_1']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_evolution_1']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_evolution_1']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_evolution_1']['field_api_classes'] = 0;
	/* Sort criterion: Content: Post date */
	$handler->display->display_options['sorts']['created']['id'] = 'created';
	$handler->display->display_options['sorts']['created']['table'] = 'node';
	$handler->display->display_options['sorts']['created']['field'] = 'created';
	$handler->display->display_options['sorts']['created']['order'] = 'DESC';
	/* Filter criterion: Content: Published */
	$handler->display->display_options['filters']['status']['id'] = 'status';
	$handler->display->display_options['filters']['status']['table'] = 'node';
	$handler->display->display_options['filters']['status']['field'] = 'status';
	$handler->display->display_options['filters']['status']['value'] = 1;
	$handler->display->display_options['filters']['status']['group'] = 1;
	$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
	/* Filter criterion: Content: Type */
	$handler->display->display_options['filters']['type']['id'] = 'type';
	$handler->display->display_options['filters']['type']['table'] = 'node';
	$handler->display->display_options['filters']['type']['field'] = 'type';
	$handler->display->display_options['filters']['type']['value'] = array(
	  'spm' => 'spm',
	);

	/* Display: Page */
	$handler = $view->new_display('page', 'Page', 'page');
	$handler->display->display_options['path'] = 'dwca-export/description-evolution/page';

	/* Display: Data export */
	$handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
	$handler->display->display_options['pager']['type'] = 'some';
	$handler->display->display_options['style_plugin'] = 'views_data_export_csv';
	$handler->display->display_options['path'] = 'dwca-export/description-evolution/csv';

	return $view;
}

function _view_description_genetics() {

	$view = new view;
	$view->name = 'dwca_export_description_genet';
	$view->description = '';
	$view->tag = 'default';
	$view->base_table = 'node';
	$view->human_name = 'DwC-A Description genetics';
	$view->core = 7;
	$view->api_version = '3.0';
	$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

	/* Display: Master */
	$handler = $view->new_display('default', 'Master', 'default');
	$handler->display->display_options['title'] = 'DwC-A Description genetics';
	$handler->display->display_options['access']['type'] = 'perm';
	$handler->display->display_options['cache']['type'] = 'none';
	$handler->display->display_options['query']['type'] = 'views_query';
	$handler->display->display_options['query']['options']['query_comment'] = FALSE;
	$handler->display->display_options['exposed_form']['type'] = 'basic';
	$handler->display->display_options['pager']['type'] = 'full';
	$handler->display->display_options['pager']['options']['items_per_page'] = '10';
	$handler->display->display_options['style_plugin'] = 'default';
	$handler->display->display_options['row_plugin'] = 'node';
	/* Relationship: Content: Taxonomy terms on node */
	$handler->display->display_options['relationships']['term_node_tid']['id'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['table'] = 'node';
	$handler->display->display_options['relationships']['term_node_tid']['field'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['required'] = 0;
	$handler->display->display_options['relationships']['term_node_tid']['vocabularies'] = array(
	  'cichorieae' => 0,
	  'image_keywords' => 0,
	  'imaging_technique' => 0,
	  'preparation_technique' => 0,
	);
	/* Field: Taxonomy term: Term ID */
	$handler->display->display_options['fields']['tid']['id'] = 'tid';
	$handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
	$handler->display->display_options['fields']['tid']['field'] = 'tid';
	$handler->display->display_options['fields']['tid']['relationship'] = 'term_node_tid';
	$handler->display->display_options['fields']['tid']['label'] = 'id';
	$handler->display->display_options['fields']['tid']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['external'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['html'] = 0;
	$handler->display->display_options['fields']['tid']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['tid']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['tid']['hide_empty'] = 0;
	$handler->display->display_options['fields']['tid']['empty_zero'] = 0;
	$handler->display->display_options['fields']['tid']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['tid']['format_plural'] = 0;
	/* Field: Content: genetics */
	$handler->display->display_options['fields']['field_genetics']['id'] = 'field_genetics';
	$handler->display->display_options['fields']['field_genetics']['table'] = 'field_data_field_genetics';
	$handler->display->display_options['fields']['field_genetics']['field'] = 'field_genetics';
	$handler->display->display_options['fields']['field_genetics']['label'] = 'type';
	$handler->display->display_options['fields']['field_genetics']['alter']['alter_text'] = 1;
	$handler->display->display_options['fields']['field_genetics']['alter']['text'] = 'genetics';
	$handler->display->display_options['fields']['field_genetics']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_genetics']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_genetics']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_genetics']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_genetics']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_genetics']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_genetics']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_genetics']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_genetics']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_genetics']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_genetics']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_genetics']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_genetics']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_genetics']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_genetics']['empty'] = 'genetics';
	$handler->display->display_options['fields']['field_genetics']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_genetics']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_genetics']['hide_alter_empty'] = 0;
	$handler->display->display_options['fields']['field_genetics']['field_api_classes'] = 0;
	/* Field: Content: genetics */
	$handler->display->display_options['fields']['field_genetics_1']['id'] = 'field_genetics_1';
	$handler->display->display_options['fields']['field_genetics_1']['table'] = 'field_data_field_genetics';
	$handler->display->display_options['fields']['field_genetics_1']['field'] = 'field_genetics';
	$handler->display->display_options['fields']['field_genetics_1']['label'] = 'description';
	$handler->display->display_options['fields']['field_genetics_1']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_genetics_1']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_genetics_1']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_genetics_1']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_genetics_1']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_genetics_1']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_genetics_1']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_genetics_1']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_genetics_1']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_genetics_1']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_genetics_1']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_genetics_1']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_genetics_1']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_genetics_1']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_genetics_1']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_genetics_1']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_genetics_1']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_genetics_1']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_genetics_1']['field_api_classes'] = 0;
	/* Sort criterion: Content: Post date */
	$handler->display->display_options['sorts']['created']['id'] = 'created';
	$handler->display->display_options['sorts']['created']['table'] = 'node';
	$handler->display->display_options['sorts']['created']['field'] = 'created';
	$handler->display->display_options['sorts']['created']['order'] = 'DESC';
	/* Filter criterion: Content: Published */
	$handler->display->display_options['filters']['status']['id'] = 'status';
	$handler->display->display_options['filters']['status']['table'] = 'node';
	$handler->display->display_options['filters']['status']['field'] = 'status';
	$handler->display->display_options['filters']['status']['value'] = 1;
	$handler->display->display_options['filters']['status']['group'] = 1;
	$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
	/* Filter criterion: Content: Type */
	$handler->display->display_options['filters']['type']['id'] = 'type';
	$handler->display->display_options['filters']['type']['table'] = 'node';
	$handler->display->display_options['filters']['type']['field'] = 'type';
	$handler->display->display_options['filters']['type']['value'] = array(
	  'spm' => 'spm',
	);

	/* Display: Page */
	$handler = $view->new_display('page', 'Page', 'page');
	$handler->display->display_options['path'] = 'dwca-export/description-genetics/page';

	/* Display: Data export */
	$handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
	$handler->display->display_options['pager']['type'] = 'some';
	$handler->display->display_options['style_plugin'] = 'views_data_export_csv';
	$handler->display->display_options['path'] = 'dwca-export/description-genetics/csv';

	return $view;
}

function _view_description_growth() {

	$view = new view;
	$view->name = 'dwca_export_description_grow';
	$view->description = '';
	$view->tag = 'default';
	$view->base_table = 'node';
	$view->human_name = 'DwC-A Description growth';
	$view->core = 7;
	$view->api_version = '3.0';
	$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

	/* Display: Master */
	$handler = $view->new_display('default', 'Master', 'default');
	$handler->display->display_options['title'] = 'DwC-A Description growth';
	$handler->display->display_options['access']['type'] = 'perm';
	$handler->display->display_options['cache']['type'] = 'none';
	$handler->display->display_options['query']['type'] = 'views_query';
	$handler->display->display_options['query']['options']['query_comment'] = FALSE;
	$handler->display->display_options['exposed_form']['type'] = 'basic';
	$handler->display->display_options['pager']['type'] = 'full';
	$handler->display->display_options['pager']['options']['items_per_page'] = '10';
	$handler->display->display_options['style_plugin'] = 'default';
	$handler->display->display_options['row_plugin'] = 'node';
	/* Relationship: Content: Taxonomy terms on node */
	$handler->display->display_options['relationships']['term_node_tid']['id'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['table'] = 'node';
	$handler->display->display_options['relationships']['term_node_tid']['field'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['required'] = 0;
	$handler->display->display_options['relationships']['term_node_tid']['vocabularies'] = array(
	  'cichorieae' => 0,
	  'image_keywords' => 0,
	  'imaging_technique' => 0,
	  'preparation_technique' => 0,
	);
	/* Field: Taxonomy term: Term ID */
	$handler->display->display_options['fields']['tid']['id'] = 'tid';
	$handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
	$handler->display->display_options['fields']['tid']['field'] = 'tid';
	$handler->display->display_options['fields']['tid']['relationship'] = 'term_node_tid';
	$handler->display->display_options['fields']['tid']['label'] = 'id';
	$handler->display->display_options['fields']['tid']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['external'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['html'] = 0;
	$handler->display->display_options['fields']['tid']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['tid']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['tid']['hide_empty'] = 0;
	$handler->display->display_options['fields']['tid']['empty_zero'] = 0;
	$handler->display->display_options['fields']['tid']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['tid']['format_plural'] = 0;
	/* Field: Content: growth */
	$handler->display->display_options['fields']['field_growth']['id'] = 'field_growth';
	$handler->display->display_options['fields']['field_growth']['table'] = 'field_data_field_growth';
	$handler->display->display_options['fields']['field_growth']['field'] = 'field_growth';
	$handler->display->display_options['fields']['field_growth']['label'] = 'type';
	$handler->display->display_options['fields']['field_growth']['alter']['alter_text'] = 1;
	$handler->display->display_options['fields']['field_growth']['alter']['text'] = 'growth';
	$handler->display->display_options['fields']['field_growth']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_growth']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_growth']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_growth']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_growth']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_growth']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_growth']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_growth']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_growth']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_growth']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_growth']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_growth']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_growth']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_growth']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_growth']['empty'] = 'growth';
	$handler->display->display_options['fields']['field_growth']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_growth']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_growth']['hide_alter_empty'] = 0;
	$handler->display->display_options['fields']['field_growth']['field_api_classes'] = 0;
	/* Field: Content: growth */
	$handler->display->display_options['fields']['field_growth_1']['id'] = 'field_growth_1';
	$handler->display->display_options['fields']['field_growth_1']['table'] = 'field_data_field_growth';
	$handler->display->display_options['fields']['field_growth_1']['field'] = 'field_growth';
	$handler->display->display_options['fields']['field_growth_1']['label'] = 'description';
	$handler->display->display_options['fields']['field_growth_1']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_growth_1']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_growth_1']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_growth_1']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_growth_1']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_growth_1']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_growth_1']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_growth_1']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_growth_1']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_growth_1']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_growth_1']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_growth_1']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_growth_1']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_growth_1']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_growth_1']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_growth_1']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_growth_1']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_growth_1']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_growth_1']['field_api_classes'] = 0;
	/* Sort criterion: Content: Post date */
	$handler->display->display_options['sorts']['created']['id'] = 'created';
	$handler->display->display_options['sorts']['created']['table'] = 'node';
	$handler->display->display_options['sorts']['created']['field'] = 'created';
	$handler->display->display_options['sorts']['created']['order'] = 'DESC';
	/* Filter criterion: Content: Published */
	$handler->display->display_options['filters']['status']['id'] = 'status';
	$handler->display->display_options['filters']['status']['table'] = 'node';
	$handler->display->display_options['filters']['status']['field'] = 'status';
	$handler->display->display_options['filters']['status']['value'] = 1;
	$handler->display->display_options['filters']['status']['group'] = 1;
	$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
	/* Filter criterion: Content: Type */
	$handler->display->display_options['filters']['type']['id'] = 'type';
	$handler->display->display_options['filters']['type']['table'] = 'node';
	$handler->display->display_options['filters']['type']['field'] = 'type';
	$handler->display->display_options['filters']['type']['value'] = array(
	  'spm' => 'spm',
	);

	/* Display: Page */
	$handler = $view->new_display('page', 'Page', 'page');
	$handler->display->display_options['path'] = 'dwca-export/description-growth/page';

	/* Display: Data export */
	$handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
	$handler->display->display_options['pager']['type'] = 'some';
	$handler->display->display_options['style_plugin'] = 'views_data_export_csv';
	$handler->display->display_options['path'] = 'dwca-export/description-growth/csv';

	return $view;
}

function _view_description_habitat() {

	$view = new view;
	$view->name = 'dwca_export_description_habit';
	$view->description = '';
	$view->tag = 'default';
	$view->base_table = 'node';
	$view->human_name = 'DwC-A Description habitat';
	$view->core = 7;
	$view->api_version = '3.0';
	$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

	/* Display: Master */
	$handler = $view->new_display('default', 'Master', 'default');
	$handler->display->display_options['title'] = 'DwC-A Description habitat';
	$handler->display->display_options['access']['type'] = 'perm';
	$handler->display->display_options['cache']['type'] = 'none';
	$handler->display->display_options['query']['type'] = 'views_query';
	$handler->display->display_options['query']['options']['query_comment'] = FALSE;
	$handler->display->display_options['exposed_form']['type'] = 'basic';
	$handler->display->display_options['pager']['type'] = 'full';
	$handler->display->display_options['pager']['options']['items_per_page'] = '10';
	$handler->display->display_options['style_plugin'] = 'default';
	$handler->display->display_options['row_plugin'] = 'node';
	/* Relationship: Content: Taxonomy terms on node */
	$handler->display->display_options['relationships']['term_node_tid']['id'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['table'] = 'node';
	$handler->display->display_options['relationships']['term_node_tid']['field'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['required'] = 0;
	$handler->display->display_options['relationships']['term_node_tid']['vocabularies'] = array(
	  'cichorieae' => 0,
	  'image_keywords' => 0,
	  'imaging_technique' => 0,
	  'preparation_technique' => 0,
	);
	/* Field: Taxonomy term: Term ID */
	$handler->display->display_options['fields']['tid']['id'] = 'tid';
	$handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
	$handler->display->display_options['fields']['tid']['field'] = 'tid';
	$handler->display->display_options['fields']['tid']['relationship'] = 'term_node_tid';
	$handler->display->display_options['fields']['tid']['label'] = 'id';
	$handler->display->display_options['fields']['tid']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['external'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['html'] = 0;
	$handler->display->display_options['fields']['tid']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['tid']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['tid']['hide_empty'] = 0;
	$handler->display->display_options['fields']['tid']['empty_zero'] = 0;
	$handler->display->display_options['fields']['tid']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['tid']['format_plural'] = 0;
	/* Field: Content: habitat */
	$handler->display->display_options['fields']['field_habitat']['id'] = 'field_habitat';
	$handler->display->display_options['fields']['field_habitat']['table'] = 'field_data_field_habitat';
	$handler->display->display_options['fields']['field_habitat']['field'] = 'field_habitat';
	$handler->display->display_options['fields']['field_habitat']['label'] = 'type';
	$handler->display->display_options['fields']['field_habitat']['alter']['alter_text'] = 1;
	$handler->display->display_options['fields']['field_habitat']['alter']['text'] = 'habitat';
	$handler->display->display_options['fields']['field_habitat']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_habitat']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_habitat']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_habitat']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_habitat']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_habitat']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_habitat']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_habitat']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_habitat']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_habitat']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_habitat']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_habitat']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_habitat']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_habitat']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_habitat']['empty'] = 'habitat';
	$handler->display->display_options['fields']['field_habitat']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_habitat']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_habitat']['hide_alter_empty'] = 0;
	$handler->display->display_options['fields']['field_habitat']['field_api_classes'] = 0;
	/* Field: Content: habitat */
	$handler->display->display_options['fields']['field_habitat_1']['id'] = 'field_habitat_1';
	$handler->display->display_options['fields']['field_habitat_1']['table'] = 'field_data_field_habitat';
	$handler->display->display_options['fields']['field_habitat_1']['field'] = 'field_habitat';
	$handler->display->display_options['fields']['field_habitat_1']['label'] = 'description';
	$handler->display->display_options['fields']['field_habitat_1']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_habitat_1']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_habitat_1']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_habitat_1']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_habitat_1']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_habitat_1']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_habitat_1']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_habitat_1']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_habitat_1']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_habitat_1']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_habitat_1']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_habitat_1']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_habitat_1']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_habitat_1']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_habitat_1']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_habitat_1']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_habitat_1']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_habitat_1']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_habitat_1']['field_api_classes'] = 0;
	/* Sort criterion: Content: Post date */
	$handler->display->display_options['sorts']['created']['id'] = 'created';
	$handler->display->display_options['sorts']['created']['table'] = 'node';
	$handler->display->display_options['sorts']['created']['field'] = 'created';
	$handler->display->display_options['sorts']['created']['order'] = 'DESC';
	/* Filter criterion: Content: Published */
	$handler->display->display_options['filters']['status']['id'] = 'status';
	$handler->display->display_options['filters']['status']['table'] = 'node';
	$handler->display->display_options['filters']['status']['field'] = 'status';
	$handler->display->display_options['filters']['status']['value'] = 1;
	$handler->display->display_options['filters']['status']['group'] = 1;
	$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
	/* Filter criterion: Content: Type */
	$handler->display->display_options['filters']['type']['id'] = 'type';
	$handler->display->display_options['filters']['type']['table'] = 'node';
	$handler->display->display_options['filters']['type']['field'] = 'type';
	$handler->display->display_options['filters']['type']['value'] = array(
	  'spm' => 'spm',
	);

	/* Display: Page */
	$handler = $view->new_display('page', 'Page', 'page');
	$handler->display->display_options['path'] = 'dwca-export/description-habitat/page';

	/* Display: Data export */
	$handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
	$handler->display->display_options['pager']['type'] = 'some';
	$handler->display->display_options['style_plugin'] = 'views_data_export_csv';
	$handler->display->display_options['path'] = 'dwca-export/description-habitat/csv';

	return $view;
}

function _view_description_life_cycle() {

	$view = new view;
	$view->name = 'dwca_export_description_life';
	$view->description = '';
	$view->tag = 'default';
	$view->base_table = 'node';
	$view->human_name = 'DwC-A Description life_cycle';
	$view->core = 7;
	$view->api_version = '3.0';
	$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

	/* Display: Master */
	$handler = $view->new_display('default', 'Master', 'default');
	$handler->display->display_options['title'] = 'DwC-A Description life_cycle';
	$handler->display->display_options['access']['type'] = 'perm';
	$handler->display->display_options['cache']['type'] = 'none';
	$handler->display->display_options['query']['type'] = 'views_query';
	$handler->display->display_options['query']['options']['query_comment'] = FALSE;
	$handler->display->display_options['exposed_form']['type'] = 'basic';
	$handler->display->display_options['pager']['type'] = 'full';
	$handler->display->display_options['pager']['options']['items_per_page'] = '10';
	$handler->display->display_options['style_plugin'] = 'default';
	$handler->display->display_options['row_plugin'] = 'node';
	/* Relationship: Content: Taxonomy terms on node */
	$handler->display->display_options['relationships']['term_node_tid']['id'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['table'] = 'node';
	$handler->display->display_options['relationships']['term_node_tid']['field'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['required'] = 0;
	$handler->display->display_options['relationships']['term_node_tid']['vocabularies'] = array(
	  'cichorieae' => 0,
	  'image_keywords' => 0,
	  'imaging_technique' => 0,
	  'preparation_technique' => 0,
	);
	/* Field: Taxonomy term: Term ID */
	$handler->display->display_options['fields']['tid']['id'] = 'tid';
	$handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
	$handler->display->display_options['fields']['tid']['field'] = 'tid';
	$handler->display->display_options['fields']['tid']['relationship'] = 'term_node_tid';
	$handler->display->display_options['fields']['tid']['label'] = 'id';
	$handler->display->display_options['fields']['tid']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['external'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['html'] = 0;
	$handler->display->display_options['fields']['tid']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['tid']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['tid']['hide_empty'] = 0;
	$handler->display->display_options['fields']['tid']['empty_zero'] = 0;
	$handler->display->display_options['fields']['tid']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['tid']['format_plural'] = 0;
	/* Field: Content: life_cycle */
	$handler->display->display_options['fields']['field_life_cycle']['id'] = 'field_life_cycle';
	$handler->display->display_options['fields']['field_life_cycle']['table'] = 'field_data_field_life_cycle';
	$handler->display->display_options['fields']['field_life_cycle']['field'] = 'field_life_cycle';
	$handler->display->display_options['fields']['field_life_cycle']['label'] = 'type';
	$handler->display->display_options['fields']['field_life_cycle']['alter']['alter_text'] = 1;
	$handler->display->display_options['fields']['field_life_cycle']['alter']['text'] = 'life_cycle';
	$handler->display->display_options['fields']['field_life_cycle']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_life_cycle']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_life_cycle']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_life_cycle']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_life_cycle']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_life_cycle']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_life_cycle']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_life_cycle']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_life_cycle']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_life_cycle']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_life_cycle']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_life_cycle']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_life_cycle']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_life_cycle']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_life_cycle']['empty'] = 'life_cycle';
	$handler->display->display_options['fields']['field_life_cycle']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_life_cycle']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_life_cycle']['hide_alter_empty'] = 0;
	$handler->display->display_options['fields']['field_life_cycle']['field_api_classes'] = 0;
	/* Field: Content: life_cycle */
	$handler->display->display_options['fields']['field_life_cycle_1']['id'] = 'field_life_cycle_1';
	$handler->display->display_options['fields']['field_life_cycle_1']['table'] = 'field_data_field_life_cycle';
	$handler->display->display_options['fields']['field_life_cycle_1']['field'] = 'field_life_cycle';
	$handler->display->display_options['fields']['field_life_cycle_1']['label'] = 'description';
	$handler->display->display_options['fields']['field_life_cycle_1']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_life_cycle_1']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_life_cycle_1']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_life_cycle_1']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_life_cycle_1']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_life_cycle_1']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_life_cycle_1']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_life_cycle_1']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_life_cycle_1']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_life_cycle_1']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_life_cycle_1']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_life_cycle_1']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_life_cycle_1']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_life_cycle_1']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_life_cycle_1']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_life_cycle_1']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_life_cycle_1']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_life_cycle_1']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_life_cycle_1']['field_api_classes'] = 0;
	/* Sort criterion: Content: Post date */
	$handler->display->display_options['sorts']['created']['id'] = 'created';
	$handler->display->display_options['sorts']['created']['table'] = 'node';
	$handler->display->display_options['sorts']['created']['field'] = 'created';
	$handler->display->display_options['sorts']['created']['order'] = 'DESC';
	/* Filter criterion: Content: Published */
	$handler->display->display_options['filters']['status']['id'] = 'status';
	$handler->display->display_options['filters']['status']['table'] = 'node';
	$handler->display->display_options['filters']['status']['field'] = 'status';
	$handler->display->display_options['filters']['status']['value'] = 1;
	$handler->display->display_options['filters']['status']['group'] = 1;
	$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
	/* Filter criterion: Content: Type */
	$handler->display->display_options['filters']['type']['id'] = 'type';
	$handler->display->display_options['filters']['type']['table'] = 'node';
	$handler->display->display_options['filters']['type']['field'] = 'type';
	$handler->display->display_options['filters']['type']['value'] = array(
	  'spm' => 'spm',
	);

	/* Display: Page */
	$handler = $view->new_display('page', 'Page', 'page');
	$handler->display->display_options['path'] = 'dwca-export/description-life_cycle/page';

	/* Display: Data export */
	$handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
	$handler->display->display_options['pager']['type'] = 'some';
	$handler->display->display_options['style_plugin'] = 'views_data_export_csv';
	$handler->display->display_options['path'] = 'dwca-export/description-life_cycle/csv';

	return $view;
}

function _view_description_life_expectancy() {

	$view = new view;
	$view->name = 'dwca_export_description_life_ex';
	$view->description = '';
	$view->tag = 'default';
	$view->base_table = 'node';
	$view->human_name = 'DwC-A Description life_expectancy';
	$view->core = 7;
	$view->api_version = '3.0';
	$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

	/* Display: Master */
	$handler = $view->new_display('default', 'Master', 'default');
	$handler->display->display_options['title'] = 'DwC-A Description life_expectancy';
	$handler->display->display_options['access']['type'] = 'perm';
	$handler->display->display_options['cache']['type'] = 'none';
	$handler->display->display_options['query']['type'] = 'views_query';
	$handler->display->display_options['query']['options']['query_comment'] = FALSE;
	$handler->display->display_options['exposed_form']['type'] = 'basic';
	$handler->display->display_options['pager']['type'] = 'full';
	$handler->display->display_options['pager']['options']['items_per_page'] = '10';
	$handler->display->display_options['style_plugin'] = 'default';
	$handler->display->display_options['row_plugin'] = 'node';
	/* Relationship: Content: Taxonomy terms on node */
	$handler->display->display_options['relationships']['term_node_tid']['id'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['table'] = 'node';
	$handler->display->display_options['relationships']['term_node_tid']['field'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['required'] = 0;
	$handler->display->display_options['relationships']['term_node_tid']['vocabularies'] = array(
	  'cichorieae' => 0,
	  'image_keywords' => 0,
	  'imaging_technique' => 0,
	  'preparation_technique' => 0,
	);
	/* Field: Taxonomy term: Term ID */
	$handler->display->display_options['fields']['tid']['id'] = 'tid';
	$handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
	$handler->display->display_options['fields']['tid']['field'] = 'tid';
	$handler->display->display_options['fields']['tid']['relationship'] = 'term_node_tid';
	$handler->display->display_options['fields']['tid']['label'] = 'id';
	$handler->display->display_options['fields']['tid']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['external'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['html'] = 0;
	$handler->display->display_options['fields']['tid']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['tid']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['tid']['hide_empty'] = 0;
	$handler->display->display_options['fields']['tid']['empty_zero'] = 0;
	$handler->display->display_options['fields']['tid']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['tid']['format_plural'] = 0;
	/* Field: Content: life_expectancy */
	$handler->display->display_options['fields']['field_life_expectancy']['id'] = 'field_life_expectancy';
	$handler->display->display_options['fields']['field_life_expectancy']['table'] = 'field_data_field_life_expectancy';
	$handler->display->display_options['fields']['field_life_expectancy']['field'] = 'field_life_expectancy';
	$handler->display->display_options['fields']['field_life_expectancy']['label'] = 'type';
	$handler->display->display_options['fields']['field_life_expectancy']['alter']['alter_text'] = 1;
	$handler->display->display_options['fields']['field_life_expectancy']['alter']['text'] = 'life_expectancy';
	$handler->display->display_options['fields']['field_life_expectancy']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_life_expectancy']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_life_expectancy']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_life_expectancy']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_life_expectancy']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_life_expectancy']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_life_expectancy']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_life_expectancy']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_life_expectancy']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_life_expectancy']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_life_expectancy']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_life_expectancy']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_life_expectancy']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_life_expectancy']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_life_expectancy']['empty'] = 'life_expectancy';
	$handler->display->display_options['fields']['field_life_expectancy']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_life_expectancy']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_life_expectancy']['hide_alter_empty'] = 0;
	$handler->display->display_options['fields']['field_life_expectancy']['field_api_classes'] = 0;
	/* Field: Content: life_expectancy */
	$handler->display->display_options['fields']['field_life_expectancy_1']['id'] = 'field_life_expectancy_1';
	$handler->display->display_options['fields']['field_life_expectancy_1']['table'] = 'field_data_field_life_expectancy';
	$handler->display->display_options['fields']['field_life_expectancy_1']['field'] = 'field_life_expectancy';
	$handler->display->display_options['fields']['field_life_expectancy_1']['label'] = 'description';
	$handler->display->display_options['fields']['field_life_expectancy_1']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_life_expectancy_1']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_life_expectancy_1']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_life_expectancy_1']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_life_expectancy_1']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_life_expectancy_1']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_life_expectancy_1']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_life_expectancy_1']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_life_expectancy_1']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_life_expectancy_1']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_life_expectancy_1']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_life_expectancy_1']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_life_expectancy_1']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_life_expectancy_1']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_life_expectancy_1']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_life_expectancy_1']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_life_expectancy_1']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_life_expectancy_1']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_life_expectancy_1']['field_api_classes'] = 0;
	/* Sort criterion: Content: Post date */
	$handler->display->display_options['sorts']['created']['id'] = 'created';
	$handler->display->display_options['sorts']['created']['table'] = 'node';
	$handler->display->display_options['sorts']['created']['field'] = 'created';
	$handler->display->display_options['sorts']['created']['order'] = 'DESC';
	/* Filter criterion: Content: Published */
	$handler->display->display_options['filters']['status']['id'] = 'status';
	$handler->display->display_options['filters']['status']['table'] = 'node';
	$handler->display->display_options['filters']['status']['field'] = 'status';
	$handler->display->display_options['filters']['status']['value'] = 1;
	$handler->display->display_options['filters']['status']['group'] = 1;
	$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
	/* Filter criterion: Content: Type */
	$handler->display->display_options['filters']['type']['id'] = 'type';
	$handler->display->display_options['filters']['type']['table'] = 'node';
	$handler->display->display_options['filters']['type']['field'] = 'type';
	$handler->display->display_options['filters']['type']['value'] = array(
	  'spm' => 'spm',
	);

	/* Display: Page */
	$handler = $view->new_display('page', 'Page', 'page');
	$handler->display->display_options['path'] = 'dwca-export/description-life_expectancy/page';

	/* Display: Data export */
	$handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
	$handler->display->display_options['pager']['type'] = 'some';
	$handler->display->display_options['style_plugin'] = 'views_data_export_csv';
	$handler->display->display_options['path'] = 'dwca-export/description-life_expectancy/csv';

	return $view;
}

function _view_description_look_alikes() {

	$view = new view;
	$view->name = 'dwca_export_description_look';
	$view->description = '';
	$view->tag = 'default';
	$view->base_table = 'node';
	$view->human_name = 'DwC-A Description look_alikes';
	$view->core = 7;
	$view->api_version = '3.0';
	$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

	/* Display: Master */
	$handler = $view->new_display('default', 'Master', 'default');
	$handler->display->display_options['title'] = 'DwC-A Description look_alikes';
	$handler->display->display_options['access']['type'] = 'perm';
	$handler->display->display_options['cache']['type'] = 'none';
	$handler->display->display_options['query']['type'] = 'views_query';
	$handler->display->display_options['query']['options']['query_comment'] = FALSE;
	$handler->display->display_options['exposed_form']['type'] = 'basic';
	$handler->display->display_options['pager']['type'] = 'full';
	$handler->display->display_options['pager']['options']['items_per_page'] = '10';
	$handler->display->display_options['style_plugin'] = 'default';
	$handler->display->display_options['row_plugin'] = 'node';
	/* Relationship: Content: Taxonomy terms on node */
	$handler->display->display_options['relationships']['term_node_tid']['id'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['table'] = 'node';
	$handler->display->display_options['relationships']['term_node_tid']['field'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['required'] = 0;
	$handler->display->display_options['relationships']['term_node_tid']['vocabularies'] = array(
	  'cichorieae' => 0,
	  'image_keywords' => 0,
	  'imaging_technique' => 0,
	  'preparation_technique' => 0,
	);
	/* Field: Taxonomy term: Term ID */
	$handler->display->display_options['fields']['tid']['id'] = 'tid';
	$handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
	$handler->display->display_options['fields']['tid']['field'] = 'tid';
	$handler->display->display_options['fields']['tid']['relationship'] = 'term_node_tid';
	$handler->display->display_options['fields']['tid']['label'] = 'id';
	$handler->display->display_options['fields']['tid']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['external'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['html'] = 0;
	$handler->display->display_options['fields']['tid']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['tid']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['tid']['hide_empty'] = 0;
	$handler->display->display_options['fields']['tid']['empty_zero'] = 0;
	$handler->display->display_options['fields']['tid']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['tid']['format_plural'] = 0;
	/* Field: Content: look_alikes */
	$handler->display->display_options['fields']['field_look_alikes']['id'] = 'field_look_alikes';
	$handler->display->display_options['fields']['field_look_alikes']['table'] = 'field_data_field_look_alikes';
	$handler->display->display_options['fields']['field_look_alikes']['field'] = 'field_look_alikes';
	$handler->display->display_options['fields']['field_look_alikes']['label'] = 'type';
	$handler->display->display_options['fields']['field_look_alikes']['alter']['alter_text'] = 1;
	$handler->display->display_options['fields']['field_look_alikes']['alter']['text'] = 'look_alikes';
	$handler->display->display_options['fields']['field_look_alikes']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_look_alikes']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_look_alikes']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_look_alikes']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_look_alikes']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_look_alikes']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_look_alikes']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_look_alikes']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_look_alikes']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_look_alikes']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_look_alikes']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_look_alikes']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_look_alikes']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_look_alikes']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_look_alikes']['empty'] = 'look_alikes';
	$handler->display->display_options['fields']['field_look_alikes']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_look_alikes']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_look_alikes']['hide_alter_empty'] = 0;
	$handler->display->display_options['fields']['field_look_alikes']['field_api_classes'] = 0;
	/* Field: Content: look_alikes */
	$handler->display->display_options['fields']['field_look_alikes_1']['id'] = 'field_look_alikes_1';
	$handler->display->display_options['fields']['field_look_alikes_1']['table'] = 'field_data_field_look_alikes';
	$handler->display->display_options['fields']['field_look_alikes_1']['field'] = 'field_look_alikes';
	$handler->display->display_options['fields']['field_look_alikes_1']['label'] = 'description';
	$handler->display->display_options['fields']['field_look_alikes_1']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_look_alikes_1']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_look_alikes_1']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_look_alikes_1']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_look_alikes_1']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_look_alikes_1']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_look_alikes_1']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_look_alikes_1']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_look_alikes_1']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_look_alikes_1']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_look_alikes_1']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_look_alikes_1']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_look_alikes_1']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_look_alikes_1']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_look_alikes_1']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_look_alikes_1']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_look_alikes_1']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_look_alikes_1']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_look_alikes_1']['field_api_classes'] = 0;
	/* Sort criterion: Content: Post date */
	$handler->display->display_options['sorts']['created']['id'] = 'created';
	$handler->display->display_options['sorts']['created']['table'] = 'node';
	$handler->display->display_options['sorts']['created']['field'] = 'created';
	$handler->display->display_options['sorts']['created']['order'] = 'DESC';
	/* Filter criterion: Content: Published */
	$handler->display->display_options['filters']['status']['id'] = 'status';
	$handler->display->display_options['filters']['status']['table'] = 'node';
	$handler->display->display_options['filters']['status']['field'] = 'status';
	$handler->display->display_options['filters']['status']['value'] = 1;
	$handler->display->display_options['filters']['status']['group'] = 1;
	$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
	/* Filter criterion: Content: Type */
	$handler->display->display_options['filters']['type']['id'] = 'type';
	$handler->display->display_options['filters']['type']['table'] = 'node';
	$handler->display->display_options['filters']['type']['field'] = 'type';
	$handler->display->display_options['filters']['type']['value'] = array(
	  'spm' => 'spm',
	);

	/* Display: Page */
	$handler = $view->new_display('page', 'Page', 'page');
	$handler->display->display_options['path'] = 'dwca-export/description-look_alikes/page';

	/* Display: Data export */
	$handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
	$handler->display->display_options['pager']['type'] = 'some';
	$handler->display->display_options['style_plugin'] = 'views_data_export_csv';
	$handler->display->display_options['path'] = 'dwca-export/description-look_alikes/csv';

	return $view;
}

function _view_description_migration() {

	$view = new view;
	$view->name = 'dwca_export_description_migr';
	$view->description = '';
	$view->tag = 'default';
	$view->base_table = 'node';
	$view->human_name = 'DwC-A Description migration';
	$view->core = 7;
	$view->api_version = '3.0';
	$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

	/* Display: Master */
	$handler = $view->new_display('default', 'Master', 'default');
	$handler->display->display_options['title'] = 'DwC-A Description migration';
	$handler->display->display_options['access']['type'] = 'perm';
	$handler->display->display_options['cache']['type'] = 'none';
	$handler->display->display_options['query']['type'] = 'views_query';
	$handler->display->display_options['query']['options']['query_comment'] = FALSE;
	$handler->display->display_options['exposed_form']['type'] = 'basic';
	$handler->display->display_options['pager']['type'] = 'full';
	$handler->display->display_options['pager']['options']['items_per_page'] = '10';
	$handler->display->display_options['style_plugin'] = 'default';
	$handler->display->display_options['row_plugin'] = 'node';
	/* Relationship: Content: Taxonomy terms on node */
	$handler->display->display_options['relationships']['term_node_tid']['id'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['table'] = 'node';
	$handler->display->display_options['relationships']['term_node_tid']['field'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['required'] = 0;
	$handler->display->display_options['relationships']['term_node_tid']['vocabularies'] = array(
	  'cichorieae' => 0,
	  'image_keywords' => 0,
	  'imaging_technique' => 0,
	  'preparation_technique' => 0,
	);
	/* Field: Taxonomy term: Term ID */
	$handler->display->display_options['fields']['tid']['id'] = 'tid';
	$handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
	$handler->display->display_options['fields']['tid']['field'] = 'tid';
	$handler->display->display_options['fields']['tid']['relationship'] = 'term_node_tid';
	$handler->display->display_options['fields']['tid']['label'] = 'id';
	$handler->display->display_options['fields']['tid']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['external'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['html'] = 0;
	$handler->display->display_options['fields']['tid']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['tid']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['tid']['hide_empty'] = 0;
	$handler->display->display_options['fields']['tid']['empty_zero'] = 0;
	$handler->display->display_options['fields']['tid']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['tid']['format_plural'] = 0;
	/* Field: Content: migration */
	$handler->display->display_options['fields']['field_migration']['id'] = 'field_migration';
	$handler->display->display_options['fields']['field_migration']['table'] = 'field_data_field_migration';
	$handler->display->display_options['fields']['field_migration']['field'] = 'field_migration';
	$handler->display->display_options['fields']['field_migration']['label'] = 'type';
	$handler->display->display_options['fields']['field_migration']['alter']['alter_text'] = 1;
	$handler->display->display_options['fields']['field_migration']['alter']['text'] = 'migration';
	$handler->display->display_options['fields']['field_migration']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_migration']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_migration']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_migration']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_migration']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_migration']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_migration']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_migration']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_migration']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_migration']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_migration']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_migration']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_migration']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_migration']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_migration']['empty'] = 'migration';
	$handler->display->display_options['fields']['field_migration']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_migration']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_migration']['hide_alter_empty'] = 0;
	$handler->display->display_options['fields']['field_migration']['field_api_classes'] = 0;
	/* Field: Content: migration */
	$handler->display->display_options['fields']['field_migration_1']['id'] = 'field_migration_1';
	$handler->display->display_options['fields']['field_migration_1']['table'] = 'field_data_field_migration';
	$handler->display->display_options['fields']['field_migration_1']['field'] = 'field_migration';
	$handler->display->display_options['fields']['field_migration_1']['label'] = 'description';
	$handler->display->display_options['fields']['field_migration_1']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_migration_1']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_migration_1']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_migration_1']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_migration_1']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_migration_1']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_migration_1']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_migration_1']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_migration_1']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_migration_1']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_migration_1']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_migration_1']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_migration_1']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_migration_1']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_migration_1']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_migration_1']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_migration_1']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_migration_1']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_migration_1']['field_api_classes'] = 0;
	/* Sort criterion: Content: Post date */
	$handler->display->display_options['sorts']['created']['id'] = 'created';
	$handler->display->display_options['sorts']['created']['table'] = 'node';
	$handler->display->display_options['sorts']['created']['field'] = 'created';
	$handler->display->display_options['sorts']['created']['order'] = 'DESC';
	/* Filter criterion: Content: Published */
	$handler->display->display_options['filters']['status']['id'] = 'status';
	$handler->display->display_options['filters']['status']['table'] = 'node';
	$handler->display->display_options['filters']['status']['field'] = 'status';
	$handler->display->display_options['filters']['status']['value'] = 1;
	$handler->display->display_options['filters']['status']['group'] = 1;
	$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
	/* Filter criterion: Content: Type */
	$handler->display->display_options['filters']['type']['id'] = 'type';
	$handler->display->display_options['filters']['type']['table'] = 'node';
	$handler->display->display_options['filters']['type']['field'] = 'type';
	$handler->display->display_options['filters']['type']['value'] = array(
	  'spm' => 'spm',
	);

	/* Display: Page */
	$handler = $view->new_display('page', 'Page', 'page');
	$handler->display->display_options['path'] = 'dwca-export/description-migration/page';

	/* Display: Data export */
	$handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
	$handler->display->display_options['pager']['type'] = 'some';
	$handler->display->display_options['style_plugin'] = 'views_data_export_csv';
	$handler->display->display_options['path'] = 'dwca-export/description-migration/csv';

	return $view;
}

function _view_description_molecular_biology() {

	$view = new view;
	$view->name = 'dwca_export_description_molec';
	$view->description = '';
	$view->tag = 'default';
	$view->base_table = 'node';
	$view->human_name = 'DwC-A Description molecular_biology';
	$view->core = 7;
	$view->api_version = '3.0';
	$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

	/* Display: Master */
	$handler = $view->new_display('default', 'Master', 'default');
	$handler->display->display_options['title'] = 'DwC-A Description molecular_biology';
	$handler->display->display_options['access']['type'] = 'perm';
	$handler->display->display_options['cache']['type'] = 'none';
	$handler->display->display_options['query']['type'] = 'views_query';
	$handler->display->display_options['query']['options']['query_comment'] = FALSE;
	$handler->display->display_options['exposed_form']['type'] = 'basic';
	$handler->display->display_options['pager']['type'] = 'full';
	$handler->display->display_options['pager']['options']['items_per_page'] = '10';
	$handler->display->display_options['style_plugin'] = 'default';
	$handler->display->display_options['row_plugin'] = 'node';
	/* Relationship: Content: Taxonomy terms on node */
	$handler->display->display_options['relationships']['term_node_tid']['id'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['table'] = 'node';
	$handler->display->display_options['relationships']['term_node_tid']['field'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['required'] = 0;
	$handler->display->display_options['relationships']['term_node_tid']['vocabularies'] = array(
	  'cichorieae' => 0,
	  'image_keywords' => 0,
	  'imaging_technique' => 0,
	  'preparation_technique' => 0,
	);
	/* Field: Taxonomy term: Term ID */
	$handler->display->display_options['fields']['tid']['id'] = 'tid';
	$handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
	$handler->display->display_options['fields']['tid']['field'] = 'tid';
	$handler->display->display_options['fields']['tid']['relationship'] = 'term_node_tid';
	$handler->display->display_options['fields']['tid']['label'] = 'id';
	$handler->display->display_options['fields']['tid']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['external'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['html'] = 0;
	$handler->display->display_options['fields']['tid']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['tid']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['tid']['hide_empty'] = 0;
	$handler->display->display_options['fields']['tid']['empty_zero'] = 0;
	$handler->display->display_options['fields']['tid']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['tid']['format_plural'] = 0;
	/* Field: Content: molecular_biology */
	$handler->display->display_options['fields']['field_molecular_biology']['id'] = 'field_molecular_biology';
	$handler->display->display_options['fields']['field_molecular_biology']['table'] = 'field_data_field_molecular_biology';
	$handler->display->display_options['fields']['field_molecular_biology']['field'] = 'field_molecular_biology';
	$handler->display->display_options['fields']['field_molecular_biology']['label'] = 'type';
	$handler->display->display_options['fields']['field_molecular_biology']['alter']['alter_text'] = 1;
	$handler->display->display_options['fields']['field_molecular_biology']['alter']['text'] = 'molecular_biology';
	$handler->display->display_options['fields']['field_molecular_biology']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_molecular_biology']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_molecular_biology']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_molecular_biology']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_molecular_biology']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_molecular_biology']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_molecular_biology']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_molecular_biology']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_molecular_biology']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_molecular_biology']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_molecular_biology']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_molecular_biology']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_molecular_biology']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_molecular_biology']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_molecular_biology']['empty'] = 'molecular_biology';
	$handler->display->display_options['fields']['field_molecular_biology']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_molecular_biology']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_molecular_biology']['hide_alter_empty'] = 0;
	$handler->display->display_options['fields']['field_molecular_biology']['field_api_classes'] = 0;
	/* Field: Content: molecular_biology */
	$handler->display->display_options['fields']['field_molecular_biology_1']['id'] = 'field_molecular_biology_1';
	$handler->display->display_options['fields']['field_molecular_biology_1']['table'] = 'field_data_field_molecular_biology';
	$handler->display->display_options['fields']['field_molecular_biology_1']['field'] = 'field_molecular_biology';
	$handler->display->display_options['fields']['field_molecular_biology_1']['label'] = 'description';
	$handler->display->display_options['fields']['field_molecular_biology_1']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_molecular_biology_1']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_molecular_biology_1']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_molecular_biology_1']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_molecular_biology_1']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_molecular_biology_1']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_molecular_biology_1']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_molecular_biology_1']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_molecular_biology_1']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_molecular_biology_1']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_molecular_biology_1']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_molecular_biology_1']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_molecular_biology_1']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_molecular_biology_1']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_molecular_biology_1']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_molecular_biology_1']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_molecular_biology_1']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_molecular_biology_1']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_molecular_biology_1']['field_api_classes'] = 0;
	/* Sort criterion: Content: Post date */
	$handler->display->display_options['sorts']['created']['id'] = 'created';
	$handler->display->display_options['sorts']['created']['table'] = 'node';
	$handler->display->display_options['sorts']['created']['field'] = 'created';
	$handler->display->display_options['sorts']['created']['order'] = 'DESC';
	/* Filter criterion: Content: Published */
	$handler->display->display_options['filters']['status']['id'] = 'status';
	$handler->display->display_options['filters']['status']['table'] = 'node';
	$handler->display->display_options['filters']['status']['field'] = 'status';
	$handler->display->display_options['filters']['status']['value'] = 1;
	$handler->display->display_options['filters']['status']['group'] = 1;
	$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
	/* Filter criterion: Content: Type */
	$handler->display->display_options['filters']['type']['id'] = 'type';
	$handler->display->display_options['filters']['type']['table'] = 'node';
	$handler->display->display_options['filters']['type']['field'] = 'type';
	$handler->display->display_options['filters']['type']['value'] = array(
	  'spm' => 'spm',
	);

	/* Display: Page */
	$handler = $view->new_display('page', 'Page', 'page');
	$handler->display->display_options['path'] = 'dwca-export/description-molecular_biology/page';

	/* Display: Data export */
	$handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
	$handler->display->display_options['pager']['type'] = 'some';
	$handler->display->display_options['style_plugin'] = 'views_data_export_csv';
	$handler->display->display_options['path'] = 'dwca-export/description-molecular_biology/csv';

	return $view;
}

function _view_description_physiology() {

	$view = new view;
	$view->name = 'dwca_export_description_phys';
	$view->description = '';
	$view->tag = 'default';
	$view->base_table = 'node';
	$view->human_name = 'DwC-A Description physiology';
	$view->core = 7;
	$view->api_version = '3.0';
	$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

	/* Display: Master */
	$handler = $view->new_display('default', 'Master', 'default');
	$handler->display->display_options['title'] = 'DwC-A Description physiology';
	$handler->display->display_options['access']['type'] = 'perm';
	$handler->display->display_options['cache']['type'] = 'none';
	$handler->display->display_options['query']['type'] = 'views_query';
	$handler->display->display_options['query']['options']['query_comment'] = FALSE;
	$handler->display->display_options['exposed_form']['type'] = 'basic';
	$handler->display->display_options['pager']['type'] = 'full';
	$handler->display->display_options['pager']['options']['items_per_page'] = '10';
	$handler->display->display_options['style_plugin'] = 'default';
	$handler->display->display_options['row_plugin'] = 'node';
	/* Relationship: Content: Taxonomy terms on node */
	$handler->display->display_options['relationships']['term_node_tid']['id'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['table'] = 'node';
	$handler->display->display_options['relationships']['term_node_tid']['field'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['required'] = 0;
	$handler->display->display_options['relationships']['term_node_tid']['vocabularies'] = array(
	  'cichorieae' => 0,
	  'image_keywords' => 0,
	  'imaging_technique' => 0,
	  'preparation_technique' => 0,
	);
	/* Field: Taxonomy term: Term ID */
	$handler->display->display_options['fields']['tid']['id'] = 'tid';
	$handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
	$handler->display->display_options['fields']['tid']['field'] = 'tid';
	$handler->display->display_options['fields']['tid']['relationship'] = 'term_node_tid';
	$handler->display->display_options['fields']['tid']['label'] = 'id';
	$handler->display->display_options['fields']['tid']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['external'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['html'] = 0;
	$handler->display->display_options['fields']['tid']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['tid']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['tid']['hide_empty'] = 0;
	$handler->display->display_options['fields']['tid']['empty_zero'] = 0;
	$handler->display->display_options['fields']['tid']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['tid']['format_plural'] = 0;
	/* Field: Content: physiology */
	$handler->display->display_options['fields']['field_physiology']['id'] = 'field_physiology';
	$handler->display->display_options['fields']['field_physiology']['table'] = 'field_data_field_physiology';
	$handler->display->display_options['fields']['field_physiology']['field'] = 'field_physiology';
	$handler->display->display_options['fields']['field_physiology']['label'] = 'type';
	$handler->display->display_options['fields']['field_physiology']['alter']['alter_text'] = 1;
	$handler->display->display_options['fields']['field_physiology']['alter']['text'] = 'physiology';
	$handler->display->display_options['fields']['field_physiology']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_physiology']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_physiology']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_physiology']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_physiology']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_physiology']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_physiology']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_physiology']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_physiology']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_physiology']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_physiology']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_physiology']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_physiology']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_physiology']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_physiology']['empty'] = 'physiology';
	$handler->display->display_options['fields']['field_physiology']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_physiology']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_physiology']['hide_alter_empty'] = 0;
	$handler->display->display_options['fields']['field_physiology']['field_api_classes'] = 0;
	/* Field: Content: physiology */
	$handler->display->display_options['fields']['field_physiology_1']['id'] = 'field_physiology_1';
	$handler->display->display_options['fields']['field_physiology_1']['table'] = 'field_data_field_physiology';
	$handler->display->display_options['fields']['field_physiology_1']['field'] = 'field_physiology';
	$handler->display->display_options['fields']['field_physiology_1']['label'] = 'description';
	$handler->display->display_options['fields']['field_physiology_1']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_physiology_1']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_physiology_1']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_physiology_1']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_physiology_1']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_physiology_1']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_physiology_1']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_physiology_1']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_physiology_1']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_physiology_1']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_physiology_1']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_physiology_1']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_physiology_1']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_physiology_1']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_physiology_1']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_physiology_1']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_physiology_1']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_physiology_1']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_physiology_1']['field_api_classes'] = 0;
	/* Sort criterion: Content: Post date */
	$handler->display->display_options['sorts']['created']['id'] = 'created';
	$handler->display->display_options['sorts']['created']['table'] = 'node';
	$handler->display->display_options['sorts']['created']['field'] = 'created';
	$handler->display->display_options['sorts']['created']['order'] = 'DESC';
	/* Filter criterion: Content: Published */
	$handler->display->display_options['filters']['status']['id'] = 'status';
	$handler->display->display_options['filters']['status']['table'] = 'node';
	$handler->display->display_options['filters']['status']['field'] = 'status';
	$handler->display->display_options['filters']['status']['value'] = 1;
	$handler->display->display_options['filters']['status']['group'] = 1;
	$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
	/* Filter criterion: Content: Type */
	$handler->display->display_options['filters']['type']['id'] = 'type';
	$handler->display->display_options['filters']['type']['table'] = 'node';
	$handler->display->display_options['filters']['type']['field'] = 'type';
	$handler->display->display_options['filters']['type']['value'] = array(
	  'spm' => 'spm',
	);

	/* Display: Page */
	$handler = $view->new_display('page', 'Page', 'page');
	$handler->display->display_options['path'] = 'dwca-export/description-physiology/page';

	/* Display: Data export */
	$handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
	$handler->display->display_options['pager']['type'] = 'some';
	$handler->display->display_options['style_plugin'] = 'views_data_export_csv';
	$handler->display->display_options['path'] = 'dwca-export/description-physiology/csv';

	return $view;
}

function _view_description_population_biology() {

	$view = new view;
	$view->name = 'dwca_export_description_popul';
	$view->description = '';
	$view->tag = 'default';
	$view->base_table = 'node';
	$view->human_name = 'DwC-A Description population_biology';
	$view->core = 7;
	$view->api_version = '3.0';
	$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

	/* Display: Master */
	$handler = $view->new_display('default', 'Master', 'default');
	$handler->display->display_options['title'] = 'DwC-A Description population_biology';
	$handler->display->display_options['access']['type'] = 'perm';
	$handler->display->display_options['cache']['type'] = 'none';
	$handler->display->display_options['query']['type'] = 'views_query';
	$handler->display->display_options['query']['options']['query_comment'] = FALSE;
	$handler->display->display_options['exposed_form']['type'] = 'basic';
	$handler->display->display_options['pager']['type'] = 'full';
	$handler->display->display_options['pager']['options']['items_per_page'] = '10';
	$handler->display->display_options['style_plugin'] = 'default';
	$handler->display->display_options['row_plugin'] = 'node';
	/* Relationship: Content: Taxonomy terms on node */
	$handler->display->display_options['relationships']['term_node_tid']['id'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['table'] = 'node';
	$handler->display->display_options['relationships']['term_node_tid']['field'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['required'] = 0;
	$handler->display->display_options['relationships']['term_node_tid']['vocabularies'] = array(
	  'cichorieae' => 0,
	  'image_keywords' => 0,
	  'imaging_technique' => 0,
	  'preparation_technique' => 0,
	);
	/* Field: Taxonomy term: Term ID */
	$handler->display->display_options['fields']['tid']['id'] = 'tid';
	$handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
	$handler->display->display_options['fields']['tid']['field'] = 'tid';
	$handler->display->display_options['fields']['tid']['relationship'] = 'term_node_tid';
	$handler->display->display_options['fields']['tid']['label'] = 'id';
	$handler->display->display_options['fields']['tid']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['external'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['html'] = 0;
	$handler->display->display_options['fields']['tid']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['tid']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['tid']['hide_empty'] = 0;
	$handler->display->display_options['fields']['tid']['empty_zero'] = 0;
	$handler->display->display_options['fields']['tid']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['tid']['format_plural'] = 0;
	/* Field: Content: population_biology */
	$handler->display->display_options['fields']['field_population_biology']['id'] = 'field_population_biology';
	$handler->display->display_options['fields']['field_population_biology']['table'] = 'field_data_field_population_biology';
	$handler->display->display_options['fields']['field_population_biology']['field'] = 'field_population_biology';
	$handler->display->display_options['fields']['field_population_biology']['label'] = 'type';
	$handler->display->display_options['fields']['field_population_biology']['alter']['alter_text'] = 1;
	$handler->display->display_options['fields']['field_population_biology']['alter']['text'] = 'population_biology';
	$handler->display->display_options['fields']['field_population_biology']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_population_biology']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_population_biology']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_population_biology']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_population_biology']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_population_biology']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_population_biology']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_population_biology']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_population_biology']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_population_biology']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_population_biology']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_population_biology']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_population_biology']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_population_biology']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_population_biology']['empty'] = 'population_biology';
	$handler->display->display_options['fields']['field_population_biology']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_population_biology']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_population_biology']['hide_alter_empty'] = 0;
	$handler->display->display_options['fields']['field_population_biology']['field_api_classes'] = 0;
	/* Field: Content: population_biology */
	$handler->display->display_options['fields']['field_population_biology_1']['id'] = 'field_population_biology_1';
	$handler->display->display_options['fields']['field_population_biology_1']['table'] = 'field_data_field_population_biology';
	$handler->display->display_options['fields']['field_population_biology_1']['field'] = 'field_population_biology';
	$handler->display->display_options['fields']['field_population_biology_1']['label'] = 'description';
	$handler->display->display_options['fields']['field_population_biology_1']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_population_biology_1']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_population_biology_1']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_population_biology_1']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_population_biology_1']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_population_biology_1']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_population_biology_1']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_population_biology_1']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_population_biology_1']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_population_biology_1']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_population_biology_1']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_population_biology_1']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_population_biology_1']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_population_biology_1']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_population_biology_1']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_population_biology_1']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_population_biology_1']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_population_biology_1']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_population_biology_1']['field_api_classes'] = 0;
	/* Sort criterion: Content: Post date */
	$handler->display->display_options['sorts']['created']['id'] = 'created';
	$handler->display->display_options['sorts']['created']['table'] = 'node';
	$handler->display->display_options['sorts']['created']['field'] = 'created';
	$handler->display->display_options['sorts']['created']['order'] = 'DESC';
	/* Filter criterion: Content: Published */
	$handler->display->display_options['filters']['status']['id'] = 'status';
	$handler->display->display_options['filters']['status']['table'] = 'node';
	$handler->display->display_options['filters']['status']['field'] = 'status';
	$handler->display->display_options['filters']['status']['value'] = 1;
	$handler->display->display_options['filters']['status']['group'] = 1;
	$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
	/* Filter criterion: Content: Type */
	$handler->display->display_options['filters']['type']['id'] = 'type';
	$handler->display->display_options['filters']['type']['table'] = 'node';
	$handler->display->display_options['filters']['type']['field'] = 'type';
	$handler->display->display_options['filters']['type']['value'] = array(
	  'spm' => 'spm',
	);

	/* Display: Page */
	$handler = $view->new_display('page', 'Page', 'page');
	$handler->display->display_options['path'] = 'dwca-export/description-population_biology/page';

	/* Display: Data export */
	$handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
	$handler->display->display_options['pager']['type'] = 'some';
	$handler->display->display_options['style_plugin'] = 'views_data_export_csv';
	$handler->display->display_options['path'] = 'dwca-export/description-population_biology/csv';

	return $view;
}

function _view_description_reproduction() {

	$view = new view;
	$view->name = 'dwca_export_description_reprod';
	$view->description = '';
	$view->tag = 'default';
	$view->base_table = 'node';
	$view->human_name = 'DwC-A Description reproduction';
	$view->core = 7;
	$view->api_version = '3.0';
	$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

	/* Display: Master */
	$handler = $view->new_display('default', 'Master', 'default');
	$handler->display->display_options['title'] = 'DwC-A Description reproduction';
	$handler->display->display_options['access']['type'] = 'perm';
	$handler->display->display_options['cache']['type'] = 'none';
	$handler->display->display_options['query']['type'] = 'views_query';
	$handler->display->display_options['query']['options']['query_comment'] = FALSE;
	$handler->display->display_options['exposed_form']['type'] = 'basic';
	$handler->display->display_options['pager']['type'] = 'full';
	$handler->display->display_options['pager']['options']['items_per_page'] = '10';
	$handler->display->display_options['style_plugin'] = 'default';
	$handler->display->display_options['row_plugin'] = 'node';
	/* Relationship: Content: Taxonomy terms on node */
	$handler->display->display_options['relationships']['term_node_tid']['id'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['table'] = 'node';
	$handler->display->display_options['relationships']['term_node_tid']['field'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['required'] = 0;
	$handler->display->display_options['relationships']['term_node_tid']['vocabularies'] = array(
	  'cichorieae' => 0,
	  'image_keywords' => 0,
	  'imaging_technique' => 0,
	  'preparation_technique' => 0,
	);
	/* Field: Taxonomy term: Term ID */
	$handler->display->display_options['fields']['tid']['id'] = 'tid';
	$handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
	$handler->display->display_options['fields']['tid']['field'] = 'tid';
	$handler->display->display_options['fields']['tid']['relationship'] = 'term_node_tid';
	$handler->display->display_options['fields']['tid']['label'] = 'id';
	$handler->display->display_options['fields']['tid']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['external'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['html'] = 0;
	$handler->display->display_options['fields']['tid']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['tid']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['tid']['hide_empty'] = 0;
	$handler->display->display_options['fields']['tid']['empty_zero'] = 0;
	$handler->display->display_options['fields']['tid']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['tid']['format_plural'] = 0;
	/* Field: Content: reproduction */
	$handler->display->display_options['fields']['field_reproduction']['id'] = 'field_reproduction';
	$handler->display->display_options['fields']['field_reproduction']['table'] = 'field_data_field_reproduction';
	$handler->display->display_options['fields']['field_reproduction']['field'] = 'field_reproduction';
	$handler->display->display_options['fields']['field_reproduction']['label'] = 'type';
	$handler->display->display_options['fields']['field_reproduction']['alter']['alter_text'] = 1;
	$handler->display->display_options['fields']['field_reproduction']['alter']['text'] = 'reproduction';
	$handler->display->display_options['fields']['field_reproduction']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_reproduction']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_reproduction']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_reproduction']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_reproduction']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_reproduction']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_reproduction']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_reproduction']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_reproduction']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_reproduction']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_reproduction']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_reproduction']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_reproduction']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_reproduction']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_reproduction']['empty'] = 'reproduction';
	$handler->display->display_options['fields']['field_reproduction']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_reproduction']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_reproduction']['hide_alter_empty'] = 0;
	$handler->display->display_options['fields']['field_reproduction']['field_api_classes'] = 0;
	/* Field: Content: reproduction */
	$handler->display->display_options['fields']['field_reproduction_1']['id'] = 'field_reproduction_1';
	$handler->display->display_options['fields']['field_reproduction_1']['table'] = 'field_data_field_reproduction';
	$handler->display->display_options['fields']['field_reproduction_1']['field'] = 'field_reproduction';
	$handler->display->display_options['fields']['field_reproduction_1']['label'] = 'description';
	$handler->display->display_options['fields']['field_reproduction_1']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_reproduction_1']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_reproduction_1']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_reproduction_1']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_reproduction_1']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_reproduction_1']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_reproduction_1']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_reproduction_1']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_reproduction_1']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_reproduction_1']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_reproduction_1']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_reproduction_1']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_reproduction_1']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_reproduction_1']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_reproduction_1']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_reproduction_1']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_reproduction_1']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_reproduction_1']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_reproduction_1']['field_api_classes'] = 0;
	/* Sort criterion: Content: Post date */
	$handler->display->display_options['sorts']['created']['id'] = 'created';
	$handler->display->display_options['sorts']['created']['table'] = 'node';
	$handler->display->display_options['sorts']['created']['field'] = 'created';
	$handler->display->display_options['sorts']['created']['order'] = 'DESC';
	/* Filter criterion: Content: Published */
	$handler->display->display_options['filters']['status']['id'] = 'status';
	$handler->display->display_options['filters']['status']['table'] = 'node';
	$handler->display->display_options['filters']['status']['field'] = 'status';
	$handler->display->display_options['filters']['status']['value'] = 1;
	$handler->display->display_options['filters']['status']['group'] = 1;
	$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
	/* Filter criterion: Content: Type */
	$handler->display->display_options['filters']['type']['id'] = 'type';
	$handler->display->display_options['filters']['type']['table'] = 'node';
	$handler->display->display_options['filters']['type']['field'] = 'type';
	$handler->display->display_options['filters']['type']['value'] = array(
	  'spm' => 'spm',
	);

	/* Display: Page */
	$handler = $view->new_display('page', 'Page', 'page');
	$handler->display->display_options['path'] = 'dwca-export/description-reproduction/page';

	/* Display: Data export */
	$handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
	$handler->display->display_options['pager']['type'] = 'some';
	$handler->display->display_options['style_plugin'] = 'views_data_export_csv';
	$handler->display->display_options['path'] = 'dwca-export/description-reproduction/csv';

	return $view;
}

function _view_description_risk_statement() {

	$view = new view;
	$view->name = 'dwca_export_description_risk';
	$view->description = '';
	$view->tag = 'default';
	$view->base_table = 'node';
	$view->human_name = 'DwC-A Description risk_statement';
	$view->core = 7;
	$view->api_version = '3.0';
	$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

	/* Display: Master */
	$handler = $view->new_display('default', 'Master', 'default');
	$handler->display->display_options['title'] = 'DwC-A Description risk_statement';
	$handler->display->display_options['access']['type'] = 'perm';
	$handler->display->display_options['cache']['type'] = 'none';
	$handler->display->display_options['query']['type'] = 'views_query';
	$handler->display->display_options['query']['options']['query_comment'] = FALSE;
	$handler->display->display_options['exposed_form']['type'] = 'basic';
	$handler->display->display_options['pager']['type'] = 'full';
	$handler->display->display_options['pager']['options']['items_per_page'] = '10';
	$handler->display->display_options['style_plugin'] = 'default';
	$handler->display->display_options['row_plugin'] = 'node';
	/* Relationship: Content: Taxonomy terms on node */
	$handler->display->display_options['relationships']['term_node_tid']['id'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['table'] = 'node';
	$handler->display->display_options['relationships']['term_node_tid']['field'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['required'] = 0;
	$handler->display->display_options['relationships']['term_node_tid']['vocabularies'] = array(
	  'cichorieae' => 0,
	  'image_keywords' => 0,
	  'imaging_technique' => 0,
	  'preparation_technique' => 0,
	);
	/* Field: Taxonomy term: Term ID */
	$handler->display->display_options['fields']['tid']['id'] = 'tid';
	$handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
	$handler->display->display_options['fields']['tid']['field'] = 'tid';
	$handler->display->display_options['fields']['tid']['relationship'] = 'term_node_tid';
	$handler->display->display_options['fields']['tid']['label'] = 'id';
	$handler->display->display_options['fields']['tid']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['external'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['html'] = 0;
	$handler->display->display_options['fields']['tid']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['tid']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['tid']['hide_empty'] = 0;
	$handler->display->display_options['fields']['tid']['empty_zero'] = 0;
	$handler->display->display_options['fields']['tid']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['tid']['format_plural'] = 0;
	/* Field: Content: risk_statement */
	$handler->display->display_options['fields']['field_risk_statement']['id'] = 'field_risk_statement';
	$handler->display->display_options['fields']['field_risk_statement']['table'] = 'field_data_field_risk_statement';
	$handler->display->display_options['fields']['field_risk_statement']['field'] = 'field_risk_statement';
	$handler->display->display_options['fields']['field_risk_statement']['label'] = 'type';
	$handler->display->display_options['fields']['field_risk_statement']['alter']['alter_text'] = 1;
	$handler->display->display_options['fields']['field_risk_statement']['alter']['text'] = 'risk_statement';
	$handler->display->display_options['fields']['field_risk_statement']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_risk_statement']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_risk_statement']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_risk_statement']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_risk_statement']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_risk_statement']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_risk_statement']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_risk_statement']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_risk_statement']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_risk_statement']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_risk_statement']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_risk_statement']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_risk_statement']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_risk_statement']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_risk_statement']['empty'] = 'risk_statement';
	$handler->display->display_options['fields']['field_risk_statement']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_risk_statement']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_risk_statement']['hide_alter_empty'] = 0;
	$handler->display->display_options['fields']['field_risk_statement']['field_api_classes'] = 0;
	/* Field: Content: risk_statement */
	$handler->display->display_options['fields']['field_risk_statement_1']['id'] = 'field_risk_statement_1';
	$handler->display->display_options['fields']['field_risk_statement_1']['table'] = 'field_data_field_risk_statement';
	$handler->display->display_options['fields']['field_risk_statement_1']['field'] = 'field_risk_statement';
	$handler->display->display_options['fields']['field_risk_statement_1']['label'] = 'description';
	$handler->display->display_options['fields']['field_risk_statement_1']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_risk_statement_1']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_risk_statement_1']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_risk_statement_1']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_risk_statement_1']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_risk_statement_1']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_risk_statement_1']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_risk_statement_1']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_risk_statement_1']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_risk_statement_1']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_risk_statement_1']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_risk_statement_1']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_risk_statement_1']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_risk_statement_1']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_risk_statement_1']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_risk_statement_1']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_risk_statement_1']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_risk_statement_1']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_risk_statement_1']['field_api_classes'] = 0;
	/* Sort criterion: Content: Post date */
	$handler->display->display_options['sorts']['created']['id'] = 'created';
	$handler->display->display_options['sorts']['created']['table'] = 'node';
	$handler->display->display_options['sorts']['created']['field'] = 'created';
	$handler->display->display_options['sorts']['created']['order'] = 'DESC';
	/* Filter criterion: Content: Published */
	$handler->display->display_options['filters']['status']['id'] = 'status';
	$handler->display->display_options['filters']['status']['table'] = 'node';
	$handler->display->display_options['filters']['status']['field'] = 'status';
	$handler->display->display_options['filters']['status']['value'] = 1;
	$handler->display->display_options['filters']['status']['group'] = 1;
	$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
	/* Filter criterion: Content: Type */
	$handler->display->display_options['filters']['type']['id'] = 'type';
	$handler->display->display_options['filters']['type']['table'] = 'node';
	$handler->display->display_options['filters']['type']['field'] = 'type';
	$handler->display->display_options['filters']['type']['value'] = array(
	  'spm' => 'spm',
	);

	/* Display: Page */
	$handler = $view->new_display('page', 'Page', 'page');
	$handler->display->display_options['path'] = 'dwca-export/description-risk_statement/page';

	/* Display: Data export */
	$handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
	$handler->display->display_options['pager']['type'] = 'some';
	$handler->display->display_options['style_plugin'] = 'views_data_export_csv';
	$handler->display->display_options['path'] = 'dwca-export/description-risk_statement/csv';

	return $view;
}

function _view_description_size() {

	$view = new view;
	$view->name = 'dwca_export_description_size';
	$view->description = '';
	$view->tag = 'default';
	$view->base_table = 'node';
	$view->human_name = 'DwC-A Description size';
	$view->core = 7;
	$view->api_version = '3.0';
	$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

	/* Display: Master */
	$handler = $view->new_display('default', 'Master', 'default');
	$handler->display->display_options['title'] = 'DwC-A Description size';
	$handler->display->display_options['access']['type'] = 'perm';
	$handler->display->display_options['cache']['type'] = 'none';
	$handler->display->display_options['query']['type'] = 'views_query';
	$handler->display->display_options['query']['options']['query_comment'] = FALSE;
	$handler->display->display_options['exposed_form']['type'] = 'basic';
	$handler->display->display_options['pager']['type'] = 'full';
	$handler->display->display_options['pager']['options']['items_per_page'] = '10';
	$handler->display->display_options['style_plugin'] = 'default';
	$handler->display->display_options['row_plugin'] = 'node';
	/* Relationship: Content: Taxonomy terms on node */
	$handler->display->display_options['relationships']['term_node_tid']['id'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['table'] = 'node';
	$handler->display->display_options['relationships']['term_node_tid']['field'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['required'] = 0;
	$handler->display->display_options['relationships']['term_node_tid']['vocabularies'] = array(
	  'cichorieae' => 0,
	  'image_keywords' => 0,
	  'imaging_technique' => 0,
	  'preparation_technique' => 0,
	);
	/* Field: Taxonomy term: Term ID */
	$handler->display->display_options['fields']['tid']['id'] = 'tid';
	$handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
	$handler->display->display_options['fields']['tid']['field'] = 'tid';
	$handler->display->display_options['fields']['tid']['relationship'] = 'term_node_tid';
	$handler->display->display_options['fields']['tid']['label'] = 'id';
	$handler->display->display_options['fields']['tid']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['external'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['html'] = 0;
	$handler->display->display_options['fields']['tid']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['tid']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['tid']['hide_empty'] = 0;
	$handler->display->display_options['fields']['tid']['empty_zero'] = 0;
	$handler->display->display_options['fields']['tid']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['tid']['format_plural'] = 0;
	/* Field: Content: size */
	$handler->display->display_options['fields']['field_size']['id'] = 'field_size';
	$handler->display->display_options['fields']['field_size']['table'] = 'field_data_field_size';
	$handler->display->display_options['fields']['field_size']['field'] = 'field_size';
	$handler->display->display_options['fields']['field_size']['label'] = 'type';
	$handler->display->display_options['fields']['field_size']['alter']['alter_text'] = 1;
	$handler->display->display_options['fields']['field_size']['alter']['text'] = 'size';
	$handler->display->display_options['fields']['field_size']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_size']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_size']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_size']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_size']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_size']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_size']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_size']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_size']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_size']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_size']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_size']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_size']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_size']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_size']['empty'] = 'size';
	$handler->display->display_options['fields']['field_size']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_size']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_size']['hide_alter_empty'] = 0;
	$handler->display->display_options['fields']['field_size']['field_api_classes'] = 0;
	/* Field: Content: size */
	$handler->display->display_options['fields']['field_size_1']['id'] = 'field_size_1';
	$handler->display->display_options['fields']['field_size_1']['table'] = 'field_data_field_size';
	$handler->display->display_options['fields']['field_size_1']['field'] = 'field_size';
	$handler->display->display_options['fields']['field_size_1']['label'] = 'description';
	$handler->display->display_options['fields']['field_size_1']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_size_1']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_size_1']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_size_1']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_size_1']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_size_1']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_size_1']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_size_1']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_size_1']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_size_1']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_size_1']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_size_1']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_size_1']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_size_1']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_size_1']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_size_1']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_size_1']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_size_1']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_size_1']['field_api_classes'] = 0;
	/* Sort criterion: Content: Post date */
	$handler->display->display_options['sorts']['created']['id'] = 'created';
	$handler->display->display_options['sorts']['created']['table'] = 'node';
	$handler->display->display_options['sorts']['created']['field'] = 'created';
	$handler->display->display_options['sorts']['created']['order'] = 'DESC';
	/* Filter criterion: Content: Published */
	$handler->display->display_options['filters']['status']['id'] = 'status';
	$handler->display->display_options['filters']['status']['table'] = 'node';
	$handler->display->display_options['filters']['status']['field'] = 'status';
	$handler->display->display_options['filters']['status']['value'] = 1;
	$handler->display->display_options['filters']['status']['group'] = 1;
	$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
	/* Filter criterion: Content: Type */
	$handler->display->display_options['filters']['type']['id'] = 'type';
	$handler->display->display_options['filters']['type']['table'] = 'node';
	$handler->display->display_options['filters']['type']['field'] = 'type';
	$handler->display->display_options['filters']['type']['value'] = array(
	  'spm' => 'spm',
	);

	/* Display: Page */
	$handler = $view->new_display('page', 'Page', 'page');
	$handler->display->display_options['path'] = 'dwca-export/description-size/page';

	/* Display: Data export */
	$handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
	$handler->display->display_options['pager']['type'] = 'some';
	$handler->display->display_options['style_plugin'] = 'views_data_export_csv';
	$handler->display->display_options['path'] = 'dwca-export/description-size/csv';

	return $view;
}

function _view_description_taxon_biology() {

	$view = new view;
	$view->name = 'dwca_export_description_taxbio';
	$view->description = '';
	$view->tag = 'default';
	$view->base_table = 'node';
	$view->human_name = 'DwC-A Description taxon_biology';
	$view->core = 7;
	$view->api_version = '3.0';
	$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

	/* Display: Master */
	$handler = $view->new_display('default', 'Master', 'default');
	$handler->display->display_options['title'] = 'DwC-A Description taxon_biology';
	$handler->display->display_options['access']['type'] = 'perm';
	$handler->display->display_options['cache']['type'] = 'none';
	$handler->display->display_options['query']['type'] = 'views_query';
	$handler->display->display_options['query']['options']['query_comment'] = FALSE;
	$handler->display->display_options['exposed_form']['type'] = 'basic';
	$handler->display->display_options['pager']['type'] = 'full';
	$handler->display->display_options['pager']['options']['items_per_page'] = '10';
	$handler->display->display_options['style_plugin'] = 'default';
	$handler->display->display_options['row_plugin'] = 'node';
	/* Relationship: Content: Taxonomy terms on node */
	$handler->display->display_options['relationships']['term_node_tid']['id'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['table'] = 'node';
	$handler->display->display_options['relationships']['term_node_tid']['field'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['required'] = 0;
	$handler->display->display_options['relationships']['term_node_tid']['vocabularies'] = array(
	  'cichorieae' => 0,
	  'image_keywords' => 0,
	  'imaging_technique' => 0,
	  'preparation_technique' => 0,
	);
	/* Field: Taxonomy term: Term ID */
	$handler->display->display_options['fields']['tid']['id'] = 'tid';
	$handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
	$handler->display->display_options['fields']['tid']['field'] = 'tid';
	$handler->display->display_options['fields']['tid']['relationship'] = 'term_node_tid';
	$handler->display->display_options['fields']['tid']['label'] = 'id';
	$handler->display->display_options['fields']['tid']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['external'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['html'] = 0;
	$handler->display->display_options['fields']['tid']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['tid']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['tid']['hide_empty'] = 0;
	$handler->display->display_options['fields']['tid']['empty_zero'] = 0;
	$handler->display->display_options['fields']['tid']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['tid']['format_plural'] = 0;
	/* Field: Content: taxon_biology */
	$handler->display->display_options['fields']['field_taxon_biology']['id'] = 'field_taxon_biology';
	$handler->display->display_options['fields']['field_taxon_biology']['table'] = 'field_data_field_taxon_biology';
	$handler->display->display_options['fields']['field_taxon_biology']['field'] = 'field_taxon_biology';
	$handler->display->display_options['fields']['field_taxon_biology']['label'] = 'type';
	$handler->display->display_options['fields']['field_taxon_biology']['alter']['alter_text'] = 1;
	$handler->display->display_options['fields']['field_taxon_biology']['alter']['text'] = 'taxon_biology';
	$handler->display->display_options['fields']['field_taxon_biology']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_taxon_biology']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_taxon_biology']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_taxon_biology']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_taxon_biology']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_taxon_biology']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_taxon_biology']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_taxon_biology']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_taxon_biology']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_taxon_biology']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_taxon_biology']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_taxon_biology']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_taxon_biology']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_taxon_biology']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_taxon_biology']['empty'] = 'taxon_biology';
	$handler->display->display_options['fields']['field_taxon_biology']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_taxon_biology']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_taxon_biology']['hide_alter_empty'] = 0;
	$handler->display->display_options['fields']['field_taxon_biology']['field_api_classes'] = 0;
	/* Field: Content: taxon_biology */
	$handler->display->display_options['fields']['field_taxon_biology_1']['id'] = 'field_taxon_biology_1';
	$handler->display->display_options['fields']['field_taxon_biology_1']['table'] = 'field_data_field_taxon_biology';
	$handler->display->display_options['fields']['field_taxon_biology_1']['field'] = 'field_taxon_biology';
	$handler->display->display_options['fields']['field_taxon_biology_1']['label'] = 'description';
	$handler->display->display_options['fields']['field_taxon_biology_1']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_taxon_biology_1']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_taxon_biology_1']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_taxon_biology_1']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_taxon_biology_1']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_taxon_biology_1']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_taxon_biology_1']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_taxon_biology_1']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_taxon_biology_1']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_taxon_biology_1']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_taxon_biology_1']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_taxon_biology_1']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_taxon_biology_1']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_taxon_biology_1']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_taxon_biology_1']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_taxon_biology_1']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_taxon_biology_1']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_taxon_biology_1']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_taxon_biology_1']['field_api_classes'] = 0;
	/* Sort criterion: Content: Post date */
	$handler->display->display_options['sorts']['created']['id'] = 'created';
	$handler->display->display_options['sorts']['created']['table'] = 'node';
	$handler->display->display_options['sorts']['created']['field'] = 'created';
	$handler->display->display_options['sorts']['created']['order'] = 'DESC';
	/* Filter criterion: Content: Published */
	$handler->display->display_options['filters']['status']['id'] = 'status';
	$handler->display->display_options['filters']['status']['table'] = 'node';
	$handler->display->display_options['filters']['status']['field'] = 'status';
	$handler->display->display_options['filters']['status']['value'] = 1;
	$handler->display->display_options['filters']['status']['group'] = 1;
	$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
	/* Filter criterion: Content: Type */
	$handler->display->display_options['filters']['type']['id'] = 'type';
	$handler->display->display_options['filters']['type']['table'] = 'node';
	$handler->display->display_options['filters']['type']['field'] = 'type';
	$handler->display->display_options['filters']['type']['value'] = array(
	  'spm' => 'spm',
	);

	/* Display: Page */
	$handler = $view->new_display('page', 'Page', 'page');
	$handler->display->display_options['path'] = 'dwca-export/description-taxon_biology/page';

	/* Display: Data export */
	$handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
	$handler->display->display_options['pager']['type'] = 'some';
	$handler->display->display_options['style_plugin'] = 'views_data_export_csv';
	$handler->display->display_options['path'] = 'dwca-export/description-taxon_biology/csv';

	return $view;
}

function _view_description_trophic_strategy() {

	$view = new view;
	$view->name = 'dwca_export_description_troph';
	$view->description = '';
	$view->tag = 'default';
	$view->base_table = 'node';
	$view->human_name = 'DwC-A Description trophic_strategy';
	$view->core = 7;
	$view->api_version = '3.0';
	$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

	/* Display: Master */
	$handler = $view->new_display('default', 'Master', 'default');
	$handler->display->display_options['title'] = 'DwC-A Description trophic_strategy';
	$handler->display->display_options['access']['type'] = 'perm';
	$handler->display->display_options['cache']['type'] = 'none';
	$handler->display->display_options['query']['type'] = 'views_query';
	$handler->display->display_options['query']['options']['query_comment'] = FALSE;
	$handler->display->display_options['exposed_form']['type'] = 'basic';
	$handler->display->display_options['pager']['type'] = 'full';
	$handler->display->display_options['pager']['options']['items_per_page'] = '10';
	$handler->display->display_options['style_plugin'] = 'default';
	$handler->display->display_options['row_plugin'] = 'node';
	/* Relationship: Content: Taxonomy terms on node */
	$handler->display->display_options['relationships']['term_node_tid']['id'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['table'] = 'node';
	$handler->display->display_options['relationships']['term_node_tid']['field'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['required'] = 0;
	$handler->display->display_options['relationships']['term_node_tid']['vocabularies'] = array(
	  'cichorieae' => 0,
	  'image_keywords' => 0,
	  'imaging_technique' => 0,
	  'preparation_technique' => 0,
	);
	/* Field: Taxonomy term: Term ID */
	$handler->display->display_options['fields']['tid']['id'] = 'tid';
	$handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
	$handler->display->display_options['fields']['tid']['field'] = 'tid';
	$handler->display->display_options['fields']['tid']['relationship'] = 'term_node_tid';
	$handler->display->display_options['fields']['tid']['label'] = 'id';
	$handler->display->display_options['fields']['tid']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['external'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['html'] = 0;
	$handler->display->display_options['fields']['tid']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['tid']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['tid']['hide_empty'] = 0;
	$handler->display->display_options['fields']['tid']['empty_zero'] = 0;
	$handler->display->display_options['fields']['tid']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['tid']['format_plural'] = 0;
	/* Field: Content: trophic_strategy */
	$handler->display->display_options['fields']['field_trophic_strategy']['id'] = 'field_trophic_strategy';
	$handler->display->display_options['fields']['field_trophic_strategy']['table'] = 'field_data_field_trophic_strategy';
	$handler->display->display_options['fields']['field_trophic_strategy']['field'] = 'field_trophic_strategy';
	$handler->display->display_options['fields']['field_trophic_strategy']['label'] = 'type';
	$handler->display->display_options['fields']['field_trophic_strategy']['alter']['alter_text'] = 1;
	$handler->display->display_options['fields']['field_trophic_strategy']['alter']['text'] = 'trophic_strategy';
	$handler->display->display_options['fields']['field_trophic_strategy']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_trophic_strategy']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_trophic_strategy']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_trophic_strategy']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_trophic_strategy']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_trophic_strategy']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_trophic_strategy']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_trophic_strategy']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_trophic_strategy']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_trophic_strategy']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_trophic_strategy']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_trophic_strategy']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_trophic_strategy']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_trophic_strategy']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_trophic_strategy']['empty'] = 'trophic_strategy';
	$handler->display->display_options['fields']['field_trophic_strategy']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_trophic_strategy']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_trophic_strategy']['hide_alter_empty'] = 0;
	$handler->display->display_options['fields']['field_trophic_strategy']['field_api_classes'] = 0;
	/* Field: Content: trophic_strategy */
	$handler->display->display_options['fields']['field_trophic_strategy_1']['id'] = 'field_trophic_strategy_1';
	$handler->display->display_options['fields']['field_trophic_strategy_1']['table'] = 'field_data_field_trophic_strategy';
	$handler->display->display_options['fields']['field_trophic_strategy_1']['field'] = 'field_trophic_strategy';
	$handler->display->display_options['fields']['field_trophic_strategy_1']['label'] = 'description';
	$handler->display->display_options['fields']['field_trophic_strategy_1']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_trophic_strategy_1']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_trophic_strategy_1']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_trophic_strategy_1']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_trophic_strategy_1']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_trophic_strategy_1']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_trophic_strategy_1']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_trophic_strategy_1']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_trophic_strategy_1']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_trophic_strategy_1']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_trophic_strategy_1']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_trophic_strategy_1']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_trophic_strategy_1']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_trophic_strategy_1']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_trophic_strategy_1']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_trophic_strategy_1']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_trophic_strategy_1']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_trophic_strategy_1']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_trophic_strategy_1']['field_api_classes'] = 0;
	/* Sort criterion: Content: Post date */
	$handler->display->display_options['sorts']['created']['id'] = 'created';
	$handler->display->display_options['sorts']['created']['table'] = 'node';
	$handler->display->display_options['sorts']['created']['field'] = 'created';
	$handler->display->display_options['sorts']['created']['order'] = 'DESC';
	/* Filter criterion: Content: Published */
	$handler->display->display_options['filters']['status']['id'] = 'status';
	$handler->display->display_options['filters']['status']['table'] = 'node';
	$handler->display->display_options['filters']['status']['field'] = 'status';
	$handler->display->display_options['filters']['status']['value'] = 1;
	$handler->display->display_options['filters']['status']['group'] = 1;
	$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
	/* Filter criterion: Content: Type */
	$handler->display->display_options['filters']['type']['id'] = 'type';
	$handler->display->display_options['filters']['type']['table'] = 'node';
	$handler->display->display_options['filters']['type']['field'] = 'type';
	$handler->display->display_options['filters']['type']['value'] = array(
	  'spm' => 'spm',
	);

	/* Display: Page */
	$handler = $view->new_display('page', 'Page', 'page');
	$handler->display->display_options['path'] = 'dwca-export/description-trophic_strategy/page';

	/* Display: Data export */
	$handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
	$handler->display->display_options['pager']['type'] = 'some';
	$handler->display->display_options['style_plugin'] = 'views_data_export_csv';
	$handler->display->display_options['path'] = 'dwca-export/description-trophic_strategy/csv';

	return $view;
}

function _view_description_uses() {

	$view = new view;
	$view->name = 'dwca_export_description_uses';
	$view->description = '';
	$view->tag = 'default';
	$view->base_table = 'node';
	$view->human_name = 'DwC-A Description uses';
	$view->core = 7;
	$view->api_version = '3.0';
	$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

	/* Display: Master */
	$handler = $view->new_display('default', 'Master', 'default');
	$handler->display->display_options['title'] = 'DwC-A Description uses';
	$handler->display->display_options['access']['type'] = 'perm';
	$handler->display->display_options['cache']['type'] = 'none';
	$handler->display->display_options['query']['type'] = 'views_query';
	$handler->display->display_options['query']['options']['query_comment'] = FALSE;
	$handler->display->display_options['exposed_form']['type'] = 'basic';
	$handler->display->display_options['pager']['type'] = 'full';
	$handler->display->display_options['pager']['options']['items_per_page'] = '10';
	$handler->display->display_options['style_plugin'] = 'default';
	$handler->display->display_options['row_plugin'] = 'node';
	/* Relationship: Content: Taxonomy terms on node */
	$handler->display->display_options['relationships']['term_node_tid']['id'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['table'] = 'node';
	$handler->display->display_options['relationships']['term_node_tid']['field'] = 'term_node_tid';
	$handler->display->display_options['relationships']['term_node_tid']['required'] = 0;
	$handler->display->display_options['relationships']['term_node_tid']['vocabularies'] = array(
	  'cichorieae' => 0,
	  'image_keywords' => 0,
	  'imaging_technique' => 0,
	  'preparation_technique' => 0,
	);
	/* Field: Taxonomy term: Term ID */
	$handler->display->display_options['fields']['tid']['id'] = 'tid';
	$handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
	$handler->display->display_options['fields']['tid']['field'] = 'tid';
	$handler->display->display_options['fields']['tid']['relationship'] = 'term_node_tid';
	$handler->display->display_options['fields']['tid']['label'] = 'id';
	$handler->display->display_options['fields']['tid']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['external'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['tid']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['tid']['alter']['html'] = 0;
	$handler->display->display_options['fields']['tid']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['tid']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['tid']['hide_empty'] = 0;
	$handler->display->display_options['fields']['tid']['empty_zero'] = 0;
	$handler->display->display_options['fields']['tid']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['tid']['format_plural'] = 0;
	/* Field: Content: uses */
	$handler->display->display_options['fields']['field_uses']['id'] = 'field_uses';
	$handler->display->display_options['fields']['field_uses']['table'] = 'field_data_field_uses';
	$handler->display->display_options['fields']['field_uses']['field'] = 'field_uses';
	$handler->display->display_options['fields']['field_uses']['label'] = 'type';
	$handler->display->display_options['fields']['field_uses']['alter']['alter_text'] = 1;
	$handler->display->display_options['fields']['field_uses']['alter']['text'] = 'uses';
	$handler->display->display_options['fields']['field_uses']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_uses']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_uses']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_uses']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_uses']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_uses']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_uses']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_uses']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_uses']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_uses']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_uses']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_uses']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_uses']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_uses']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_uses']['empty'] = 'uses';
	$handler->display->display_options['fields']['field_uses']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_uses']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_uses']['hide_alter_empty'] = 0;
	$handler->display->display_options['fields']['field_uses']['field_api_classes'] = 0;
	/* Field: Content: uses */
	$handler->display->display_options['fields']['field_uses_1']['id'] = 'field_uses_1';
	$handler->display->display_options['fields']['field_uses_1']['table'] = 'field_data_field_uses';
	$handler->display->display_options['fields']['field_uses_1']['field'] = 'field_uses';
	$handler->display->display_options['fields']['field_uses_1']['label'] = 'description';
	$handler->display->display_options['fields']['field_uses_1']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['field_uses_1']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['field_uses_1']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['field_uses_1']['alter']['external'] = 0;
	$handler->display->display_options['fields']['field_uses_1']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['field_uses_1']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['field_uses_1']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['field_uses_1']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['field_uses_1']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['field_uses_1']['alter']['more_link'] = 0;
	$handler->display->display_options['fields']['field_uses_1']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['field_uses_1']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['field_uses_1']['alter']['html'] = 0;
	$handler->display->display_options['fields']['field_uses_1']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['field_uses_1']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['field_uses_1']['hide_empty'] = 0;
	$handler->display->display_options['fields']['field_uses_1']['empty_zero'] = 0;
	$handler->display->display_options['fields']['field_uses_1']['hide_alter_empty'] = 1;
	$handler->display->display_options['fields']['field_uses_1']['field_api_classes'] = 0;
	/* Sort criterion: Content: Post date */
	$handler->display->display_options['sorts']['created']['id'] = 'created';
	$handler->display->display_options['sorts']['created']['table'] = 'node';
	$handler->display->display_options['sorts']['created']['field'] = 'created';
	$handler->display->display_options['sorts']['created']['order'] = 'DESC';
	/* Filter criterion: Content: Published */
	$handler->display->display_options['filters']['status']['id'] = 'status';
	$handler->display->display_options['filters']['status']['table'] = 'node';
	$handler->display->display_options['filters']['status']['field'] = 'status';
	$handler->display->display_options['filters']['status']['value'] = 1;
	$handler->display->display_options['filters']['status']['group'] = 1;
	$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
	/* Filter criterion: Content: Type */
	$handler->display->display_options['filters']['type']['id'] = 'type';
	$handler->display->display_options['filters']['type']['table'] = 'node';
	$handler->display->display_options['filters']['type']['field'] = 'type';
	$handler->display->display_options['filters']['type']['value'] = array(
	  'spm' => 'spm',
	);

	/* Display: Page */
	$handler = $view->new_display('page', 'Page', 'page');
	$handler->display->display_options['path'] = 'dwca-export/description-uses/page';

	/* Display: Data export */
	$handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
	$handler->display->display_options['pager']['type'] = 'some';
	$handler->display->display_options['style_plugin'] = 'views_data_export_csv';
	$handler->display->display_options['path'] = 'dwca-export/description-uses/csv';

	return $view;
}


