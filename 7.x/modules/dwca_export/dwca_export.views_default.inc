<?php

/**
 * Implementation of hook_views_default_views().
 *
 * @see dwca_export_views_api()
 */
function dwca_export_views_default_views() {
	static $view_functions = array('classification');

	$views = array();

	foreach ($view_functions as $function_postfix) {
		$function = '_view_' . $function_postfix;
		$view = $function();
		$views[$view->name] = $view;
	}
	return $views;
}

/**
 * Classification Export View - exported from views module
 */
function _view_classification() {

	$view = new view;
	$view->name = 'classification_export_view';
	$view->description = '';
	$view->tag = 'default';
	$view->base_table = 'taxonomy_term_data';
	$view->human_name = 'Classification Export View';
	$view->core = 7;
	$view->api_version = '3.0-alpha1';
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
	/* Field: Taxonomy term: Name */
	$handler->display->display_options['fields']['name']['id'] = 'name';
	$handler->display->display_options['fields']['name']['table'] = 'taxonomy_term_data';
	$handler->display->display_options['fields']['name']['field'] = 'name';
	$handler->display->display_options['fields']['name']['label'] = '';
	$handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['name']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['name']['alter']['word_boundary'] = 0;
	$handler->display->display_options['fields']['name']['alter']['ellipsis'] = 0;
	$handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['name']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['name']['alter']['html'] = 0;
	$handler->display->display_options['fields']['name']['hide_empty'] = 0;
	$handler->display->display_options['fields']['name']['empty_zero'] = 0;
	$handler->display->display_options['fields']['name']['link_to_taxonomy'] = 1;
	/* Field: Taxonomy term: Term ID */
	$handler->display->display_options['fields']['tid']['id'] = 'tid';
	$handler->display->display_options['fields']['tid']['table'] = 'taxonomy_term_data';
	$handler->display->display_options['fields']['tid']['field'] = 'tid';
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
	/* Field: Taxonomy vocabulary: Name */
	$handler->display->display_options['fields']['name_2']['id'] = 'name_2';
	$handler->display->display_options['fields']['name_2']['table'] = 'taxonomy_vocabulary';
	$handler->display->display_options['fields']['name_2']['field'] = 'name';
	$handler->display->display_options['fields']['name_2']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['name_2']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['name_2']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['name_2']['alter']['external'] = 0;
	$handler->display->display_options['fields']['name_2']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['name_2']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['name_2']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['name_2']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['name_2']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['name_2']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['name_2']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['name_2']['alter']['html'] = 0;
	$handler->display->display_options['fields']['name_2']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['name_2']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['name_2']['hide_empty'] = 0;
	$handler->display->display_options['fields']['name_2']['empty_zero'] = 0;
	$handler->display->display_options['fields']['name_2']['hide_alter_empty'] = 0;
	/* Field: Taxonomy term: Name */
	$handler->display->display_options['fields']['name_3']['id'] = 'name_3';
	$handler->display->display_options['fields']['name_3']['table'] = 'taxonomy_term_data';
	$handler->display->display_options['fields']['name_3']['field'] = 'name';
	$handler->display->display_options['fields']['name_3']['relationship'] = 'parent';
	$handler->display->display_options['fields']['name_3']['label'] = 'ParentName';
	$handler->display->display_options['fields']['name_3']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['name_3']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['name_3']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['name_3']['alter']['external'] = 0;
	$handler->display->display_options['fields']['name_3']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['name_3']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['name_3']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['name_3']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['name_3']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['name_3']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['name_3']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['name_3']['alter']['html'] = 0;
	$handler->display->display_options['fields']['name_3']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['name_3']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['name_3']['hide_empty'] = 0;
	$handler->display->display_options['fields']['name_3']['empty_zero'] = 0;
	$handler->display->display_options['fields']['name_3']['hide_alter_empty'] = 0;
	$handler->display->display_options['fields']['name_3']['link_to_taxonomy'] = 0;
	/* Field: Taxonomy term: Term ID */
	$handler->display->display_options['fields']['tid_1']['id'] = 'tid_1';
	$handler->display->display_options['fields']['tid_1']['table'] = 'taxonomy_term_data';
	$handler->display->display_options['fields']['tid_1']['field'] = 'tid';
	$handler->display->display_options['fields']['tid_1']['relationship'] = 'parent';
	$handler->display->display_options['fields']['tid_1']['label'] = 'ParentId';
	$handler->display->display_options['fields']['tid_1']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['tid_1']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['tid_1']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['tid_1']['alter']['external'] = 0;
	$handler->display->display_options['fields']['tid_1']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['tid_1']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['tid_1']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['tid_1']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['tid_1']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['tid_1']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['tid_1']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['tid_1']['alter']['html'] = 0;
	$handler->display->display_options['fields']['tid_1']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['tid_1']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['tid_1']['hide_empty'] = 0;
	$handler->display->display_options['fields']['tid_1']['empty_zero'] = 0;
	$handler->display->display_options['fields']['tid_1']['hide_alter_empty'] = 0;
	$handler->display->display_options['fields']['tid_1']['separator'] = '';
	$handler->display->display_options['fields']['tid_1']['format_plural'] = 0;
	/* Field: Taxonomy vocabulary: Description */
	$handler->display->display_options['fields']['description']['id'] = 'description';
	$handler->display->display_options['fields']['description']['table'] = 'taxonomy_vocabulary';
	$handler->display->display_options['fields']['description']['field'] = 'description';
	$handler->display->display_options['fields']['description']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['description']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['description']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['description']['alter']['external'] = 0;
	$handler->display->display_options['fields']['description']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['description']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['description']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['description']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['description']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['description']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['description']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['description']['alter']['html'] = 0;
	$handler->display->display_options['fields']['description']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['description']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['description']['hide_empty'] = 0;
	$handler->display->display_options['fields']['description']['empty_zero'] = 0;
	$handler->display->display_options['fields']['description']['hide_alter_empty'] = 0;
	/* Field: Taxonomy term: Term description */
	$handler->display->display_options['fields']['description_1']['id'] = 'description_1';
	$handler->display->display_options['fields']['description_1']['table'] = 'taxonomy_term_data';
	$handler->display->display_options['fields']['description_1']['field'] = 'description';
	$handler->display->display_options['fields']['description_1']['alter']['alter_text'] = 0;
	$handler->display->display_options['fields']['description_1']['alter']['make_link'] = 0;
	$handler->display->display_options['fields']['description_1']['alter']['absolute'] = 0;
	$handler->display->display_options['fields']['description_1']['alter']['external'] = 0;
	$handler->display->display_options['fields']['description_1']['alter']['replace_spaces'] = 0;
	$handler->display->display_options['fields']['description_1']['alter']['trim_whitespace'] = 0;
	$handler->display->display_options['fields']['description_1']['alter']['nl2br'] = 0;
	$handler->display->display_options['fields']['description_1']['alter']['word_boundary'] = 1;
	$handler->display->display_options['fields']['description_1']['alter']['ellipsis'] = 1;
	$handler->display->display_options['fields']['description_1']['alter']['strip_tags'] = 0;
	$handler->display->display_options['fields']['description_1']['alter']['trim'] = 0;
	$handler->display->display_options['fields']['description_1']['alter']['html'] = 0;
	$handler->display->display_options['fields']['description_1']['element_label_colon'] = 1;
	$handler->display->display_options['fields']['description_1']['element_default_classes'] = 1;
	$handler->display->display_options['fields']['description_1']['hide_empty'] = 0;
	$handler->display->display_options['fields']['description_1']['empty_zero'] = 0;
	$handler->display->display_options['fields']['description_1']['hide_alter_empty'] = 0;
	/* Sort criterion: Taxonomy term: Term ID */
	$handler->display->display_options['sorts']['tid']['id'] = 'tid';
	$handler->display->display_options['sorts']['tid']['table'] = 'taxonomy_term_data';
	$handler->display->display_options['sorts']['tid']['field'] = 'tid';
	/* Filter criterion: Taxonomy vocabulary: Machine name */
	$handler->display->display_options['filters']['machine_name']['id'] = 'machine_name';
	$handler->display->display_options['filters']['machine_name']['table'] = 'taxonomy_vocabulary';
	$handler->display->display_options['filters']['machine_name']['field'] = 'machine_name';
	$handler->display->display_options['filters']['machine_name']['value'] = array(
	  'dwca_export_test_taxonomy' => 'dwca_export_test_taxonomy',
	);

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