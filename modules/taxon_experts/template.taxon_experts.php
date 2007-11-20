<?php
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

  if(user_access('create expertsdb_interest content')){
    $add_op_link = l(t('Add Interest'),'node/add/expertsdb_interest', null, drupal_get_destination().'&field_parent_person='._taxon_experts_get_profile_nid('expertsbd_person'));
  }
  
  $operations = array('view');
  if(user_access('edit expertsdb_interest content')){
    $operations[] = 'edit';
    $operations[] = 'delete';
  }
  
  return theme('views_view_optable', $view, $nodes, 'table', $add_op_link, $operations);
}

//address_by_contact
function phptemplate_views_view_table_address_by_contact($view, $nodes){

  if(user_access('create expertsdb_address content')){
    $add_op_link = l(t('Add Address'),'node/add/expertsdb_address', null, drupal_get_destination().'&field_parent_contact='._taxon_experts_get_profile_nid('expertsdb_contact'));
  }
  
  $operations = array('view');
  if(user_access('edit expertsdb_address content')){
    $operations[] = 'edit';
    $operations[] = 'delete';
  }
  
  $output .= theme('views_view_optable', $view, $nodes, 'table', $add_op_link, $operations);

  return $output;
}

//projects_by_interest
function phptemplate_views_view_table_projects_by_interest($view, $nodes){

  if(user_access('create expertsdb_project content')){
    $add_op_link = l(t('Add Project'),'node/60/edit', null, drupal_get_destination().'#field_taxon_project');
  }
  
  $operations = array();
  if(user_access('edit expertsdb_project')){
    $operations[] = 'edit';
    $operations[] = 'delete';
  }
  
  $output .= theme('views_view_optable', $view, $nodes, 'table', $add_op_link, $operations);

  return $output;
}


/*
 * Theme function for the nodeprofile display as teaser
 */
function phptemplate_nodeprofile_display_teaser(&$element) {

  if ($node = nodeprofile_load($element['#content_type'], $element['#uid'])) {
    
    switch($element['#content_type']){
      
      case 'expertsbd_person':
        
        $node = node_build_content($node, $teaser, $page);
        
        $output = '<div class="person">';
        //$output = '<h2 class="person">'.l($node->title, 'node/'.$node->nid).'&nbsp;<span class="operations">['. l('edit', 'node/'.$node->nid.'/edit', null, drupal_get_destination()).']</span></h2>';
        //$output = '<h2 class="person">'.l($node->title, 'node/'.$node->nid).'</h2>';
        $output .= '<h2 class="title">'.$node->title;
        
        $aliases = array();
        foreach($node->field_name_aliases as $entry){
          $aliases[] =  $entry['value'];
        }
        if( count($aliases) && !empty($aliases[0])){
          $output .= ' <span class="name_aliases">('.join(', ', $aliases).')</span>';
        }
        $output .= '</h2>'; // end of h2.title    
        
        if( !(isset($node->field_lifespan['#access']) && $node->field_lifespan['#access']=== false) ){
          $output .= $node->field_lifespan[0]['view'];
        }
        
        if($node->field_retirement_year[0]['value'] && !(isset($node->field_retirement_year['#access']) && $node->field_retirement_year['#access'] === false) ) {
           $output .= '<div class="field_retirement_year">'.t('Foreseen Year of Retirement:').'&nbsp;'.$node->field_retirement_year[0]['view'].'</div>'.chr(10);
        }
        $output .= '</div><div class="clear-float"></div>';// end of div.person
       
        $view = views_get_view('interest_by_uid');
        $output .= '<h3 class="title">'.t('Taxonomic Interests').'</h3>';
        $output .= '<dl><dd class="nodeprofile-nodeprofile-display"><div class="nodeprofile-display">';
        $output .= views_build_view('block', $view, array($node->nid));        
        $output .= '</dl></dd></div>';
        
        return $output;
        
      case 'expertsdb_contact':
        $view = views_get_view('address_by_contact');
        // remove all tabs
        unset($element['#tabs']);
        // themed in theme_nodeprofile_display_box() file /modules/nodeprofile/nodeprofile.module
        $content_addon = views_build_view('block', $view, array($node->nid) );

        
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
          $output .= $items['expertsbd_person']['value'];
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


function phptemplate_date_display_combination($field, $dates, $node = NULL) {
    switch ($field['field_name']){
      case 'field_lifespan': //field_retirement_year  
        return theme_date_lifespan($field, $dates, $node);
        
      default:  
        return theme_date_display_combination($field, $dates, $node);
    }
}

function phptemplate_views_handle_field_search_person_node_title($fields, $field, $data){
  $field['handler'] = 'taxon_experts_handler_user_link';
  return theme_views_handle_field($fields, $field, $data);
}


function phptemplate_views_filters($form) {
  switch($form['#view_name']){
    
    case 'search_interest':
      
      static $filter_layout = array(
      	'term_node_12.tid' => // taxa
            array(
      			'row' => 0,
                'colspan' => false,
            	'label_position' => 'inline-first',
            	'operator_position' => 'inline-first',
            ),
         'node.title' => // person name
            array(
      			'row' => 0,
                'colspan' => false,
            	'label_position' => 'inline-first',
            	'operator_position' => 'inline-first',
            ),   
         
        'term_node_13.tid' => // geography
            array(
      			'row' => 1,
                'colspan' => false,
            	'label_position' => 'inline-first',
            	'operator_position' => 'inline-first',
            ),
        'term_node_10.tid' => // environment
            array(
      			'row' => 1,
                'colspan' => false,
            	'label_position' => 'inline-first',
            	'operator_position' => 'inline-first',
            ),
        'term_node_6.tid' => // activity
            array(
      			'row' => 2,
                'colspan' => false,
            	'label_position' => 'inline-first',
            	'operator_position' => 'inline-first',
            ),
        'term_node_5.tid' => // methodology
            array(
      			'row' => 2,
                'colspan' => false,
            	'label_position' => 'inline-first',
            	'operator_position' => 'inline-first',
            ),
      );
      
      $view = $form['view']['#value'];
      foreach ($view->exposed_filter as $count => $expose) {
        $layout = $filter_layout[$expose['field']];
        $rows[$layout['row']][] = '<h3>'.$expose['label'].'</h3>'.drupal_render($form["op$count"]) . drupal_render($form["filter$count"]);
      }
      
      $rows[count($rows)][] = drupal_render($form['submit']);
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


?>