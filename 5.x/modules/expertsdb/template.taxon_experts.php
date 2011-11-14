<?php
// $Id$


/*
 * Copyright (C) 2007 EDIT
 * European Distributed Institute of Taxonomy
 * http://www.e-taxonomy.eu
 *
 */

/**
 function _init_vocabulary_tids(){
 //FIXME: execute this during module installation
 //FIXME: check for  variable_get('expertsdb_vocabs') && tids are valid for system status
 $expertsdb_vocabs = array(
 'expertdb_taxomony' =>	12,
 'expertdb_georegions' => 13,
 'expertdb_environment' => 10,
 'expertdb_activity' =>	6,
 'expertdb_methods' => 5,
 );

 taxonomy_get_vocabulary()

 variable_set('expertsdb_vocabs', $expertsdb_vocabs);
 }
 */



/**
 * Template for BLOCK of view 'taxonomic_interest_by_uid'
 *
 * @param unknown_type $view
 * @param unknown_type $nodes
 */
function phptemplate_views_view_table_interest_by_uid($view, $nodes){

	// allow creation of interests only on own profile page
	if((user_access('edit own expertsdb_interest content') && _taxon_experts_user_is_owner()) || user_access('edit expertsdb_interest content')){
		$add_op_link = l(t('Add Interest'),'node/add/expertsdb_interest', null, drupal_get_destination().'&field_parent_person='._taxon_experts_get_profile_nid('expertsdb_person'));
	}

	$operations = array('view');
	if((user_access('edit own expertsdb_interest content') && _taxon_experts_user_is_owner()) || user_access('edit expertsdb_interest content')){
		$operations[] = t('edit');
		$operations[] = t('delete');
	}

	return theme('views_view_optable', $view, $nodes, 'table', $add_op_link, $operations);
}


//projects_by_interest
function phptemplate_views_view_table_projects_by_interest($view, $nodes){

	// allow creation of projects only on own profile page
	if((user_access('edit own expertsdb_project content') && _taxon_experts_user_is_owner()) || user_access('edit expertsdb_project content')){
		$add_op_link = l(t('Add Project'),'node/60/edit', null, drupal_get_destination().'#field_taxon_project');
	}

	$operations = array('view');
	if((user_access('edit own expertsdb_project content') && _taxon_experts_user_is_owner()) || user_access('edit expertsdb_project content')){
		$operations[] = t('edit');
		$operations[] = t('delete');
	}

	return theme('views_view_optable', $view, $nodes, 'table', $add_op_link, $operations);

}

/**
 * Template for BLOCK of view 'institutional_memberships_by_uid'
 *
 * @param unknown_type $view
 * @param unknown_type $nodes
 */
function phptemplate_views_view_table_institutional_memberships_by_uid($view, $nodes){

	// allow creation of memberships only on own profile page
	if((user_access('edit own expertsdb_instmembership content') && _taxon_experts_user_is_owner()) || user_access('edit expertsdb_instmembership content')){
		$add_op_link = l(t('Add Institutional Membership'),'node/add/expertsdb-instmembership', null, drupal_get_destination().'&field_parent_member='._taxon_experts_get_profile_nid('expertsdb_person'));
	}

	$operations = array('view');
	if((user_access('edit own expertsdb_instmembership content') && _taxon_experts_user_is_owner()) || user_access('edit expertsdb_instmembership content')){
		$operations[] = t('edit');
		$operations[] = t('delete');
	}

	return theme('views_view_optable', $view, $nodes, 'table', $add_op_link, $operations);
}

/**
 * Template for BLOCK of view 'members_by_institute'
 *
 * @param unknown_type $view
 * @param unknown_type $nodes
 */
function phptemplate_views_view_table_members_by_institute($view, $nodes){

	return theme('views_view_optable', $view, $nodes, 'table', NULL, NULL);
}

/*
 * Theme function for the nodeprofile display as teaser
 */
function phptemplate_nodeprofile_display_teaser(&$element) {

	if ($node = nodeprofile_load($element['#content_type'], $element['#uid'])) {

		// pull in the stylesheet
		drupal_add_css(drupal_get_path('module', 'taxon_experts') .'/taxon_experts_view.css');
  			  		
		switch($element['#content_type']){

			case 'expertsdb_person':
				// render profile node as FULL nodes, not as teaser
				$node = node_build_content($node, FALSE, FALSE);
				
				// render edit link
				$output .=  _taxon_experts_menu_custom($node->nid, $element['#uid']);
			
				$output .= '<div class="nodeprofile-person">';
				//$output = '<h2 class="person">'.l($node->title, 'node/'.$node->nid).'&nbsp;<span class="operations">['. l('edit', 'node/'.$node->nid.'/edit', null, drupal_get_destination()).']</span></h2>';
				//$output = '<h2 class="person">'.l($node->title, 'node/'.$node->nid).'</h2>';
				$output .= '<h2 class="title">' . l($node->title, 'user/'.$node->uid);

				// append name aliases
				if(!empty($node->field_name_aliases[0])){
					$output .= ' <span class="name-aliases">' . $node->field_name_aliases[0]['view'] . '</span>';
				}

				$output .= '</h2>'; // end of h2.title

				$output .= '<div class="nodeprofile-dates">';

				// display lifespan
				if( $node->field_lifespan && !empty($node->field_lifespan[0]['timespan_from']) && !(isset($node->field_lifespan['#access']) && $node->field_lifespan['#access']=== false) ){
					$output .= $node->field_lifespan[0]['view'];
				}

				// display retirement year
				if($node->field_retirement_year && !empty($node->field_retirement_year[0]['retirement_year']) && !empty($node->field_retirement_year[0]['view']) && !(isset($node->field_retirement_year['#access']) && $node->field_retirement_year['#access'] === false) ) {
					// Change widget label, if retirement year is in the past (Person already retired)
					$label = ($node->field_retirement_year[0]['retirement_year'] < date('Y')) ? t('Year of Retirement:') : t('Foreseen Year of Retirement:');
					$output .= '<div class="field-retirement-year"><span class="label">' . $label . '</span>&nbsp;'.$node->field_retirement_year[0]['view'].'</div>'.chr(10);
				}

				$output .= '</div>'; // end of div.dates

				// render complete contact from nodeprofile
				$output .= drupal_render($node->content['group_contact']);

				$output .= '</div>'; // end of div.person

				// clear block
				$output .= '<div class="clear-float"> </div>';

				// configure views
				$views = array(
        	'institutional_memberships_by_uid' => array(
        		'title' => t('Institutional Memberships')
				),
        	'interest_by_uid' => array(
        		'title' => t('Taxonomic Interests'),
				),
				);

				foreach($views as $view_name => $view_settings){
					$view = views_get_view($view_name);
					if(views_access($view)){
						// get view data
						$view_data = views_build_view('block', $view, array($node->nid));
						if(!$view_data){
							// provide an empty table, if there is no data
							$view_data = theme('views_view_table_' . $view_name, $view, array());
						}
						$output .= '<h3 class="title">' . $view_settings['title'] . '</h3>';
						$output .= '<div class="nodeprofile-display">';
						$output .= $view_data;
						$output .= '</div>';
					}
				}

				// clear block
				$output .= '<div class="clear-float"> </div>';

				return $output;


			default:
				// copied from theme_nodeprofile_display_teaser
				if (node_access('view', $node)) {
					$element['#node'] = $node;
					return theme('nodeprofile_display_box', $element, node_view($node, TRUE, TRUE, FALSE).$content_addon);
				}
				//# end default
		}//# end switch
	}
}


/**
 * Theme a user page
 * @param $account the user object
 * @param $fields a multidimensional array for the fields, in the form of array (
 *   'category1' => array(item_array1, item_array2), 'category2' => array(item_array3,
 *    .. etc.). Item arrays are formatted as array(array('title' => 'item title',
 * 'value' => 'item value', 'class' => 'class-name'), ... etc.). Module names are incorporated
 * into the CSS class.
 */
function phptemplate_user_profile($account, $fields) {
	$output = '<div class="profile">';
	$output .= theme('user_picture', $account);

	foreach ($fields as $category => $items) {
		switch ($category){
			case 'Person':
				$output .= $items['expertsdb_person']['value'];
				break;
			default:
				if (strlen($category) > 0) {
					$output .= '<h3 class="title">'.$category .'</h3>';
				}
				$output .= '<dl>';
				foreach ($items as $item) {
					if (isset($item['title'])) {
						$output .= '<dt class="'. $item['class'] .'">'. $item['title'] .'</dt>';
					}
					$output .= '<dd class="'. $item['class'] .'">'. $item['value'] .'</dd>';
				}
				$output .= '</dl>';
		}
	}
	$output .= '</div>';

	return $output;
}


/*function phptemplate_date_display_combination($field, $dates, $node = NULL) {
 switch ($field['field_name']){
 case 'field_lifespan': //field_retirement_year
 return theme_date_lifespan($field, $dates, $node);

 default:
 return theme_date_display_combination($field, $dates, $node);
 }
 }*/

/*
 * Theming function to override the field_handler for Views Fusion node titles
 *
 * Attention: vX depends on the fused view. Set Breakpoint in views.module, line 1268
 *
 */
function phptemplate_views_handle_field_v4node_title($fields, $field, $data){
	switch($field['label']){
		case "Person":
			$field['handler'] = 'taxon_experts_handler_user_link';
			return theme_views_handle_field($fields, $field, $data);

		default:
			return theme_views_handle_field($fields, $field, $data);
	}
}

/*
 * handler for node title link in search_person view
 */
function phptemplate_views_handle_field_search_person_node_title($fields, $field, $data){
	$field['handler'] = 'taxon_experts_handler_user_link';
	return theme_views_handle_field($fields, $field, $data);
}

/*
 * handler for parent_member field
 */
function phptemplate_views_handle_field_node_data_field_parent_member_field_parent_member_nid($fields, $field, $data){
	$field['handler'] = 'taxon_experts_handler_expert_link';
	return theme_views_handle_field($fields, $field, $data, $value);
}

/*
 * handler for parent_person field
 */
function phptemplate_views_handle_field_node_data_field_parent_person_field_parent_person_nid($fields, $field, $data){
	$field['handler'] = 'taxon_experts_handler_expert_link';
	return theme_views_handle_field($fields, $field, $data, $value);
}

/*
 * handle node id fields; in search_interests this is the interest itself
 */
function phptemplate_views_handle_field_node_nid($fields, $field, $data){
	$field['handler'] = 'taxon_experts_handler_user_interest_link';
	return theme_views_handle_field($fields, $field, $data, $value);
}

/*
 * handler for person link in institution_by_ref view
 */
function phptemplate_views_handle_field_parent_member_field_parent_member_nid($fields, $field, $data){
	$field['handler'] = 'taxon_experts_handler_user_link';
	return theme_views_handle_field($fields, $field, $data);
}


/*
 * function to format serialized expertsdb_timespan fields for field_membership_period
 *
 */
function phptemplate_views_handle_field_node_data_field_membership_period_field_membership_period_expertsdb_timespan($fields, $field, $data){
	$field['handler'] = 'expertsdb_timespan_handler_view';
	return theme_views_handle_field($fields, $field, $data);
}

/*
 * function to format serialized expertsdb_timespan fields for field_lifespan
 *
 */
function phptemplate_views_handle_field_node_data_field_lifespan_field_lifespan_expertsdb_timespan($fields, $field, $data){
	$field['handler'] = 'expertsdb_timespan_handler_view';
	return theme_views_handle_field($fields, $field, $data);
}

function phptemplate_views_filters($form) {
	switch($form['#view_name']){

		case 'search_interest':
			// pull in the style sheet
			drupal_add_css(drupal_get_path('module', 'taxon_experts') .'/taxon_experts_search_interests.css');

			// set order of exposed filter bei label, as the field names may be different
			// Filter labels may be different as well, so filter labels must be consistent with labels in the view,
			// but maintainig filter labels consitent seems to be much easier than maintaining field names
			$num_cols = 1;
			$filter_order = array(
				'Person' => 0,
    			'Field of Taxonomic Expertise' => 1,
    			'Geografical Range' => 2,
    			'Environment' => 4,
    			'Activity' => 5,
    			'Methodology' => 6,
			);

			// provide filter descriptions for each filter
			$filter_descripton = array(
				'Person' => '',
    			'Field of Taxonomic Expertise' => t('To <strong>select items</strong>, click the items in the <em>left box</em>. To <strong>deselect items</strong>, click the items in the <em>right box</em>.'),
    			'Geografical Range' => t('To <strong>select items</strong>, click the items in the <em>left box</em>. To <strong>deselect items</strong>, click the items in the <em>right box</em>.'),
    			'Environment' => t('Hold <strong>Ctrl</strong> or <strong>Strg</strong> to select multiple items or deselect items.'),
    			'Activity' => t('Hold <strong>Ctrl</strong> or <strong>Strg</strong> to select multiple items or deselect items.'),
    			'Methodology' => t('Hold <strong>Ctrl</strong> or <strong>Strg</strong> to select multiple items or deselect items.'),
			);

			$view = $form['view']['#value'];
			// build the filter layout
			foreach($view->exposed_filter as $delta => $filter){
				/*
				 * Hide filter options on taxonomy and georegions
				 *
				 */

				if($filter['label'] == 'Field of Taxonomic Expertise' || $filter['label'] == 'Geografical Range'){
					$form["op$delta"]['#type'] = 'hidden';
					$form["op$delta"]['#default_value'] = 'AND';
				}
				/* Tweak the filter options in the Person filter
				 *
				 * @TODO: Verify, that this way of theming the filter options is appropriate;
				 * There *might* be another way of theming the selection
				 */
				if($filter['label'] == 'Person'){
					$allowed_filters = array('contains','not');
					if($form["op$delta"] && $form["op$delta"]['#options']){
						foreach($form["op$delta"]['#options'] as $key => $text){
							if(!in_array($key,$allowed_filters)) unset ($form["op$delta"]['#options'][$key]);
						}
					}

					/*
					 * Bugfix:
					 * Due to an unknown reason, the default value from the person filter ($form["filter$delta"]['#value'])
					 * is set to an empty array instead of an empty string - which is very annoying.
					 * This is a workaround
					 *
					 * @TODO: remove the next lines and track down the bug
					 */
					if (is_array($form["filter$delta"]['#value']) && count($form["filter$delta"]['#value']) == 0){
						$form["filter$delta"]['#value'] = '';
					}

				}

				/*
				 * Build collapsible fieldsets for all filters
				 */

				if($_GET['filter'.$delta]){
					$used_filter = ' (active)';
					$state = '';
				}
				else{
					$used_filter = '';
					$state = ' collapsed';
				}

				$form['op'.$delta]['#weight'] = $filter_order[$filter['label']]*10;
				$form['op'.$delta]['#prefix'] = '<fieldset class="collapsible' . $state . '"><legend>Filter ' . $filter['label'] . $used_filter . '</legend>';
				$form['op'.$delta]['#prefix'] .= '<p>' .  $filter_descripton[$filter['label']] . '</p>';
				$form['filter'.$delta]['#weight'] = $filter_order[$filter['label']]*10+1;
				$form['filter'.$delta]['#suffix'] = '</fieldset>';

			}

			// add the new submit button
			$form['submit'] = array(
    		'#type' => 'views_imagebutton',
				'#name' => 'submit',
				'#title' => t('Click to filter results'),
    			'#image' => drupal_get_path('module','cdm_taxontree') . '/images/filter_results_button.png',  // provide the path to your image here
    			'#default_value' => t('Submit'), // original value of button text
				'#weight' => count($view->exposed_filter)*10+2,
				'#id' => 'submit_filters'
			);

			$form['reset'] = array(
    		'#type' => 'resetbutton',
				'#name' => 'reset',
				'#title' => t('Click to reset all filters'),
    			'#image' => drupal_get_path('module','cdm_taxontree') . '/images/reset_filters_button.png',  // provide the path to your image here
    			'#default_value' => t('Reset'), // original value of button text
				'#weight' => count($view->exposed_filter)*10+3,
				'#url' => $view->url,
			);

			// add the collapse script
			drupal_add_js('misc/collapse.js');

			return drupal_render($form);


		case 'search_person':
			$view = $form['view']['#value'];
			foreach($view->exposed_filter as $delta => $filter){

				/* Tweak the filter options in the Person filter
				 *
				 * @TODO: Verify, that this way of theming the filter options is appropriate;
				 * There *might* be another way of theming the selection
				 */
				if($filter['label'] == 'Person'){
					$allowed_filters = array('contains','not');
					if($form["op$delta"] && $form["op$delta"]['#options']){
						foreach($form["op$delta"]['#options'] as $key => $text){
							if(!in_array($key,$allowed_filters)) unset ($form["op$delta"]['#options'][$key]);
						}
					}
				}
			}

			foreach ($view->exposed_filter as $count => $expose) {
				$layout = $filter_layout[$expose['field']];
				$rows[$layout['row']][] = '<div class="field-label">'.$expose['label'].'</div>'.drupal_render($form["op$count"]) . drupal_render($form["filter$count"]);
			}

			$form['submit'] = array(
    		'#type' => 'views_imagebutton',
				'#name' => 'submit',
				'#title' => t('Click to filter results'),
    			'#image' => drupal_get_path('module','cdm_taxontree') . '/images/filter_results_button.png',  // provide the path to your image here
    			'#default_value' => t('Submit'), // original value of button text
			);

			$rows[count($rows)][] = array(
				'data' => drupal_render($form['submit']),
				'colspan' => $num_cols,
				'class' => 'row-submit',
			);
			$label[] = ''; // so the column count is the same.
			return theme('table', null, $rows) . drupal_render($form);
			break;

		default:
			return theme_views_filters($form);
	}
}

function phptemplate_field(&$node, &$field, &$items, $teaser, $page) {
	$field_empty = TRUE;
	foreach ($items as $delta => $item) {
		if (!empty($item['view']) || $item['view'] === "0") {
			$field_empty = FALSE;
			break;
		}
	}

	$variables = array(
    'node' => $node,
    'field' => $field,
    'field_type' => $field['type'],
    'field_name' => $field['field_name'],
    'field_type_css' => strtr($field['type'], '_', '-'),
    'field_name_css' => strtr($field['field_name'], '_', '-'),
    'label' => t($field['widget']['label']),
    'label_display' => isset($field['display_settings']['label']['format']) ? $field['display_settings']['label']['format'] : 'above',
    'field_empty' => $field_empty,
    'items' => $items,
    'teaser' => $teaser,
    'page' => $page,
	);

	return _phptemplate_callback('field', $variables, array('field-'. $field['field_name']));
}


/**
 * Custom form element create an image based reset button (which is actually a link)
 */
function resetbutton_elements() {
  $type['resetbutton'] = array('#input' => TRUE, '#button_type' => 'reset',);
  return $type;
}

function theme_resetbutton($element) {
  return '<a href="' .url($element['#url']) . '" title="' . $element['#title'] . '"><img src="' . $element['#image'] . '" name="'. $element['#name'] .'" alt="' . $element['#title'] . '" ' . drupal_attributes($element['#attributes']) .' /></a>';
}

function resetbutton_value() {
  // null function guarantees default_value doesn't get moved to #value.
}

