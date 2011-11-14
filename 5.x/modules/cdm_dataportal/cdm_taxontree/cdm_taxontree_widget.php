<?php


/*
 * Definition of privacy levels
 */
define('TAXONPRIVACY_PRIVATE', 'Private');
define('TAXONPRIVACY_PUBLIC', 'Public');

/**
 * Implementation of hook_perm()
 *
 * Valid permissions for this module
 * @return array An array of valid permissions for the taxon_experts module
 */
function cdm_taxontree_perm() {
    return array(
        'view private taxoninterests',
    //TODO which else permission are required?
    );
}


/**
 * Implementation of hook_field()
 *
 */
function cdm_taxontree_field($op, &$node, $field, &$items, $teaser, $page) {
    switch ($op) {

        case 'view':
            $context = $teaser ? 'teaser' : 'full';
            $formatter = isset($field['display_settings'][$context]['format']) ? $field['display_settings'][$context]['format'] : 'default';
            foreach ($items as $delta => $item) {
                $items[$delta]['view'] = content_format($field, $item, $formatter, $node);
            }
            return theme('field', $node, $field, $items, $teaser, $page);
    }
}


/**
 * Implementation of hook_field_settings().
 */
function cdm_taxontree_field_settings($op, $field) {
    switch ($op) {
        case 'form':
            $form = array(
        '#theme' => 'cdm_taxontree_field_settings',
            );
            return $form;

        case 'save':
            break;

        case 'database columns':
            return array(
        'tid' => array('type' => 'int', 'length' => 10, 'unsigned' => TRUE, 'not null' => TRUE, 'default' => 0),
            );
            break;

        case 'filters':
            return array(
             'default' => array(
       'operator' => 'views_handler_operator_like',
       'handler' => 'cdm_taxontree_views_filter_handler',
            ),
            );
            break;

        case 'callbacks': //pairs up with cck_fullname_field::view
            return array(
        'view' => CONTENT_CALLBACK_CUSTOM,
            );
            break;

    }
}

/**
 * implementation of hook_widget_info()
 *
 */
function cdm_taxontree_widget_info(){
    $vocabularies = taxonomy_get_vocabularies();
    $vocabs = array();
    foreach ($vocabularies as $vid => $vocab) {
        $vocabs['cdm_taxontree_' .$vocab->vid] = array(
        'label' => $vocab->name,
        'field types' => array('cdm_taxontree'),
        );
    }
    return $vocabs;
}

/**
 * implementation of hook_widget()
 *
 */
function cdm_taxontree_widget($op, &$node, $field, &$node_field){
    switch($op){
        case 'prepare form values':
            // get posted values in both node edit and profile edit mode
            if ($_POST[$field['field_name']] || $_POST[$field['type_name'].'_node_form'][$field['field_name']]) {
                $node_field = ($_POST['form_id'] == 'user_edit') ?  $_POST[$field['type_name'].'_node_form'][$field['field_name']] : $_POST[$field['field_name']];
            }else{
                $node_field_transposed = content_transpose_array_rows_cols($node_field);
                $node_field = array();
                $node_field[0]['cdm_taxontree_select'] = $node_field_transposed['tid'];
            }
            return;

        case 'form':
            $form = array();
            $form[$field['field_name']] = array(
        '#tree' => TRUE,
        '#theme' => NULL,
        '#type' => 'markup',
        '#title' => t($field['widget']['label']),
        '#description' => t($field['widget']['description']),
        '#weight' => $field['widget']['weight'],
            );

            // add the form
            $delta = 0;
            _cdm_taxontree_widget_form($form[$field['field_name']][$delta], $field, $node_field);

            return $form;

        case 'validate':
            if($field['required'] && count($node_field[0]['cdm_taxontree_select']) < 1){
                form_set_error('cdm_taxontree_select', t('Please select at least one item from %type.', array('%type' => $field['widget']['label'])));
            }
         return;
      }
    }

function _cdm_taxontree_widget_form(&$form_item, $field = array(), $node_field = array(), $delta = 0) {

  // get vocabulary id
  $vid = substr($field['widget']['type'], strlen($field['type'])+1);

  // build the tree
  $tree = cdm_taxontree_build_tree(NULL, NULL, $vid);

  $options = array();
  if (!empty($node_field[$delta]['cdm_taxontree_select'])){
    $options = _cdm_taxontree_recreate_options($node_field[$delta]['cdm_taxontree_select']);
  }

  // prefix and suffix: render the taxon tree div structure around the select field
  $prefix .= '<div class="cdm_taxontree_widget">';
  // hide labels in filter view
  if(substr($field['field_name'],-7,7) !== '_filter'){
    $prefix .= '<div class="taxontree_header">';
    $prefix .= '<div class="field-label column-left">' . t($field['widget']['label']) . ':' . ($field['required'] == 1 ? ' <span class="form-required" title="' .t('This field is required.'). '">*</span>' : '') . '</div>';
    $prefix .= '<div class="field-label column-right">' . t('Your Selection') . ':</div>';
    $prefix .= '</div>';
  }
  $prefix .= '<div class="taxontree">';
  $prefix .= '<div class="cdm_taxontree_scroller_x">';
  $prefix .= '<div class="cdm_taxontree_container">';
  $prefix .= '<div class="cdm_taxontree_scroller_y">';
  // inject the taxon tree
  $prefix .= theme('cdm_taxontree',$tree, NULL, FALSE, 'cdm_taxontree_node_reference_widget', $field['field_name']);
  $prefix .= '</div></div></div></div>';
  $prefix .= '<div class="taxontree_devider"><span class="hidden">&gt;&gt;</span></div>';
  // this is the place where to put the select box without disturbing the scrollers
  $suffix = '</div>';

  // pull in the default value
  $default_value = (!empty($field['widget']['default_value'][$delta]['cdm_taxontree_select'])) ? $field['widget']['default_value'][$delta]['cdm_taxontree_select'] : array();
  // $default_show_childnodes = ($field['widget']['default_value']['include_childnodes']['include_childnodes'] === 'include_childnodes') ? 'checked="checked"' : '';
  $default_show_childnodes = 'checked="checked"';

  // add the select box and set to multiple, if appropriate
  $form_item['cdm_taxontree_select'] = array(
        '#tree' => TRUE,
        '#type' => 'select',
        '#default_value' => !empty($node_field[$delta]['cdm_taxontree_select']) ? $node_field[$delta]['cdm_taxontree_select'] : $default_value ,
        '#multiple' => $field['multiple'] ? TRUE : FALSE,
        '#options' => $options,
        '#size' => $field['multiple'] ? 12 : 2,
        '#prefix' => $prefix,
        '#suffix' => $suffix,
        '#attributes' => array('class' => 'taxontree_select'),
  );

  // add the scripts for multiple widgets, if not already present
  theme('cdm_taxontree_add_scripts');

  global $taxontree_script_present;
  if(!$taxontree_script_present[$field['field_name']]){
    drupal_add_js('$(document).ready(function() {$(\'ul.cdm_taxontree.' . $field['field_name'] . '\').cdm_taxontree(
      {
        widget:                 true,
        element_name:           \''.$field['field_name'] .'\',  //
        multiselect:            '.($field['multiple']?'true':'false').'         //
      }
      );});', 'inline');
    $taxontree_script_present[$field['field_name']] = TRUE;
  }

  // add flag for including child nodes if selected in settings
  // this is only tirggered in filter mode
  if($field['show_include_childnodes']){

    $default_value = (count($_GET)==1) ? $default_show_childnodes : ($field['include_childnodes_flag']) ? 'checked="checked"' : '';

    $include_label = 'Include Childnodes';
    $include_childnode_message = 'Include childnodes and parent node';
    // HACK! Replace the text to display as 'include childnodes text'
    if(substr($field['field_name'],-7,7) == '_filter'){
      $vocabulary = substr($field['widget']['type'],strlen($field['type'])+1);
      switch($vocabulary){

        case 8:
          $include_label = 'Include Childtaxa';
          $include_childnode_message = 'Check, if all child taxa of the selected taxa shall be included';
          break;
        case 3:
          $include_label = 'Include Subregions';
          $include_childnode_message = 'Check, if all subregions of the selected region should be included';
          break;

      }
    }

    $form_item['include_childnodes'] = array(
      '#tree' => TRUE,
      '#type' => 'markup',
      '#prefix' => '<div class="include-options">',
      '#value' => '<label class="field-label" for="' . $field['field_name'] . $delta . '-include">' . t($include_label) . '</label><div class="field-description">' . t($include_childnode_message) . '<input type="checkbox" name="'  .$field['field_name'] . $delta . '_include" id="' . $field['field_name'] . $delta . '-include" value="include_childnodes" ' . $default_value . '/></div>',
      '#suffix' => '</div>',
    );

  }

}


/**
 * implementation of hook_filters_alter(), a *views modification*
 * to enable injection of more filter values
 *
 * @param object $view, needed to find cdm_taxontree filters by $delta
 * @param array $filters, incoming filter values
 * @return array $filters extended filter values
 */
function cdm_taxontree_views_filters_alter($view, $filters){

    if($view->name == 'search_interest'){
        // find the cdm_taxontree_filter
        foreach($view->exposed_filter as $delta => $filter){
            if(preg_match('@^cdm_taxontree@',$filter['field'])){

                // include child terms and parent term
                if($_GET['cdm_taxontree_filter' . $delta . '_include'] == TRUE && is_array($filters[$delta]['filter'])){
                    foreach($filters[$delta]['filter'] as $key => $val){

                        if(is_numeric($val)){
                            $child_terms[] = $val;
                        }
                    }

                    $term_collection = array();
                    // get child terms from cache
                    foreach($child_terms as $key => $term){
                        if(!$children = cdm_taxontree_cache_get($term,'cache_cdm_taxontree')){
                            // no data in cache? add some!
                            $child_terms = _cdm_taxontree_get_all_children($child_terms,TRUE);
                            // add parent term
                            $parent_terms = taxonomy_get_parents($term);
                            foreach($parent_terms as $key => $term_obj){
                                $child_terms[$key] = $key;
                            }

                            cdm_taxontree_cache_set($term,'cache_cdm_taxontree',serialize($child_terms));
                            $children = $child_terms;
                        }
                        else{
                            $children = unserialize($children->data);
                        }

                        $term_collection = array_merge($term_collection, $children);
                    }

                    // re-add filter values
                    $filters[$delta]['filter'] = $term_collection;

                }
            }
        }
    }
                
    /*
     * React on taxonprivacy settings
     * Remove taxon_privacy from taxonomy / filters, if user is allowed to view private taxon interests
     */
    global $user;

    // show information only to roles with access to private interests
    if($user->uid && user_access('view private taxoninterests',$user)){
            foreach($view->filter as $delta => $filter){
                if(preg_match('@^term_node_@',$filter['field'])){
                    $tid = intval(str_replace('.tid','',str_replace('term_node_','',$filter['field'])));
                    if($tid == 9){
                        unset($view->filter[$delta]);
                    }
                }
            }
    }

    
    return $filters;
}


/*
 * implementation of hook_form_alter()
 *
 * used to replace the standard taxonomy selectbox
 * with our nice cdm_taxontree_widget form.
 *
 */
function cdm_taxontree_form_alter($form_id, &$form){

    if($form_id == 'views_filters')
    {
        global $user;
        $view = $form['view']['#value'];

        // find the cdm_taxontree_filter
        foreach($view->exposed_filter as $delta => $filter){
            if(preg_match('@^cdm_taxontree@',$filter['field'])){

                // simulate field settings to invoke the widget form correctly
                $field = array(
                    'field_name' => 'cdm_taxontree_filter',
                    'type' => 'cdm_taxontree',
                  'multiple' => $form['filter'.$delta]['#multiple'],
                    'show_include_childnodes' => 1,
                    'include_childnodes_flag' => $_GET['cdm_taxontree_filter' . $delta . '_include'] ? 1 : 0,
                    'widget' => array(
                        'type' => 'cdm_taxontree_' . $form['filter'.$delta]['#vocab'],
                        'label' => $filter['label'],
                ),
                );

                // simulate the node_field data from filter form values
                $filter_field = array();
                $filter_values = $form['filter'.$delta]['#default_value'];

                // transpose the filter values to a form understood by the widget
                if(is_array($filter_values)){
                    foreach($filter_values as $key => $val){
                        $filter_field[$delta]['cdm_taxontree_select'][] = $val;
                    }
                }else if(is_numeric($key)){
                    $filter_field[$delta]['cdm_taxontree_select'][] = $filter_values;
                }

                // create the widget
                $subform = array();
                _cdm_taxontree_widget_form($subform['cdm_taxontree_filter'], $field, $filter_field, $delta);

                // add some important attributes
                $subform['cdm_taxontree_filter']['cdm_taxontree_select']['#name'] = 'filter'.$delta;
                $subform['cdm_taxontree_filter']['cdm_taxontree_select']['#id'] = 'edit-filter'.$delta;

                // replace original form element with widget
                $form['filter'.$delta] = $subform['cdm_taxontree_filter'];

            }
        }
    }
}


/**
 * Implematation of hook_views_tables()
 *
 *
 * @return onject views related db table data
 */
function cdm_taxontree_views_tables() {
    $tables = array();
    // field_name, type_name, widget_type
    $result = db_query("SELECT nfi.*, nf.db_storage FROM {node_field_instance} nfi INNER JOIN {node_field} nf ON nfi.field_name = nf.field_name WHERE widget_type LIKE 'cdm_taxontree_%'");
    while ($row = db_fetch_object($result)) {

        // Build the list of options
        $vid = substr($row->widget_type, 14); // 14 = strlen('cdm_taxontree)!

        $options = array();
        $tree = taxonomy_form($vid);
        foreach ($tree['#options'] as $index => $option) {
            if (is_object($option)) {
                foreach($option->option as $key => $value) {
                    $options[$key] = $value;
                }
            }
        }

        // Rely on CCK to provide us with the correct table name.
        if ($row->db_storage == CONTENT_DB_STORAGE_PER_FIELD) {
            $table_name = _content_tablename($row->field_name, CONTENT_DB_STORAGE_PER_FIELD);
        }
        else {
            $table_name = _content_tablename($row->type_name, CONTENT_DB_STORAGE_PER_CONTENT_TYPE);
        }

        # Get the taxonomy multi select property
        $vocab = taxonomy_get_vocabulary($vid);
        $multiple = $vocab->multiple;

        $table = array(
      'name' => $table_name,
      'provider' => 'cdm_taxontree',
      'join' => array(
        'left' => array('table' => 'node', 'field' => 'vid',),
        'right' => array('field' => 'vid'),
        ),
      'filters' => array(
        $row->field_name .'_tid' => array(
          'name' => t('CDM Taxontree: @field_name', array('@field_name' => $row->field_name)),
          'help' => t('Filter on @field_name terms.', array('@field_name' => $row->field_name)),
          'operator' => 'views_handler_operator_or',
                    'handler' => 'cdm_taxontree_views_handler_filter_default',
          'value' => array(
            '#type' => 'select',
        // pass the vocabulary id
                        '#vocab' => $vid,
            '#options' => $options,
            '#multiple' => $multiple,
        ),
        ),
        ),
        );
        $tables['cdm_taxontree'. $row->field_name] = $table;
    }
    return $tables;
}

function cdm_taxontree_views_handler_filter_default($op, $filter, $filterinfo, &$query) {
    $table = $filterinfo['table'];
    $field = $filterinfo['field'];

    if (is_array($filter['value']) && count($filter['value'])) {
        if ($filter['operator'] == 'OR' || $filter['operator'] == 'NOR') {
            $query->ensure_table($table);
            $where_args = array_merge(array($query->use_alias_prefix . $table, $field), $filter['value']);
            $placeholder = array_fill(0, count($filter['value']), '%s');
            if ($filter['operator'] == 'OR') {
                $query->add_where("%s.%s IN (". implode(", ", $placeholder) .")", $where_args);
            }
            else {
                $where_args[] = $where_args[0];
                $where_args[] = $where_args[1];
                $query->add_where("(%s.%s NOT IN (". implode(", ", $placeholder) .") OR %s.%s IS NULL)", $where_args);
            }
        }
        else {
            $howmany = count($filter['value']);
            $high_table = $query->add_table($table, true, $howmany);
            if (!$high_table) { // couldn't add the table
                return;
            }

            $table_num = $high_table - $howmany;
            foreach ($filter['value'] as $item) {
                $table_num++;
                $tn = $query->get_table_name($table, $table_num);
                $query->add_where("%s.%s = '%s'", $tn, $field, $item);
            }
        }
    }
    else {
        $query->ensure_table("$table");
        $query->add_where("%s.%s %s '%s'", $query->use_alias_prefix . $table, $field, $filter['operator'], $filter['value']);
    }
}

/**
 * implematation of hook_token_list()
 *
 */
function cdm_taxontree_token_list($type = 'all') {
    if ($type == 'field' || $type == 'all') {
        $tokens = array();

        $tokens['cdm_taxontree']['tid'] = t("Term ID - first item only.");
        $tokens['cdm_taxontree']['tid-all'] = t("Term ID - all items comma separated");
        $tokens['cdm_taxontree']['tid-n'] = t("Term ID - where n = the value to select if more than one value in the field.");
        $tokens['cdm_taxontree']['raw'] = t("Raw, unfiltered term - first item only");
        $tokens['cdm_taxontree']['raw-all'] = t("Raw, unfiltered terms - all items comma separated");
        $tokens['cdm_taxontree']['raw-n'] = t('Raw, unfiltered terms - where n = the value to select if more than one value in the field');
        $tokens['cdm_taxontree']['formatted'] = t("Formatted and filtered terms - first item only");
        $tokens['cdm_taxontree']['formatted-all'] = t("Formatted and filtered terms - all items with line break separated");
        $tokens['cdm_taxontree']['formatted-n'] = t('Formatted and filtered terms - where n = the value to select if more than one value in the field');

        return $tokens;
    }
}


/**
 * implementation of hook_token_values()
 *
 */
function cdm_taxontree_token_values($type, $object = NULL) {
    if ($type == 'field') {
        $i = 0;
        foreach ($object as $value) {
            $term = taxonomy_get_term($value['tid']);
            $values[] = $term->name;
            $views[] = $value['view'];
            $tokens['raw-' . $i] = $term->name;
            $tokens['formatted-' . $i] = $value['view'];
            $i++;
        }

        //for backwards compat
        $tokens['raw-all']  = implode(', ' , $values);
        $tokens['formatted-all'] = implode("\n", $views);

        return $tokens;
    }
}

/**
 * Helper function to transpose a list of term ids into an
 * array with key = tid and value = term name;
 * used to recreate cdm_taxontree_widgets select list options.
 *
 * @param array $options
 * @return array transposed options
 */
function _cdm_taxontree_recreate_options($options = array()){
    $transposed = array();
    foreach($options as $key => $tid){
        if(is_numeric($tid)){
            $parents = taxonomy_get_parents_all($tid);
            $path = theme('cdm_taxontree_taxon_fullpath',array_reverse($parents),3);
            $transposed[$tid] = $path;
        }
    }
    return $transposed;
}


/**
 * Theme function that returns a <span> element containing the secundum of a given TreeNode> element
 *
 * @param array $node The <TreeNode> element as returned by the webservice
 *
 * @return String html formatted title of the secundum
 * @deprecated replace by according taxonomictree function
 */
function theme_cdm_taxontree_node_reference(&$node){

  $secRefTitle = cdm_taxontree_secRefTitle_for($node->secUuid);
  // encode any special characters
  $secRefTitle = check_plain($secRefTitle);
  $out = ' <span class="sec_ref widget_select" title="'.$secRefTitle.'" style="background-color:#'._uuid_to_rgbhex($node->secUuid).'" alt="'.$node->secUuid.'">'
      .$secRefTitle.'</span>';
  return $out;
}


/**
 * theme function to display terms in the cd,_taxontree_widget with additional data.
 *
 */
function theme_cdm_taxontree_node_reference_widget(&$node){
    $title = ($node->titleCache) ? check_plain($node->titleCache) : check_plain($node->name);
    $parents = taxonomy_get_parents_all($node->tid);
    $path = theme('cdm_taxontree_taxon_fullpath',array_reverse($parents),3);
    $out = ' <span class="sec_ref widget_select" title="' . (!empty($path) ? $path : $title) . '" alt="'.$node->tid.'">' . $title . '</span>';
    return $out;
}

/**
 * theme function to replace term names with full path to term name
 *
 * @param array $tids path->to->term
 * @param integer $max_levels, 0 is unlimited
 * @return text representation of path->to->term
 */
function theme_cdm_taxontree_taxon_fullpath($tids = array(), $max_levels = 3){
    $path = array();
    foreach ($tids as $tid){
        $path[] = $tid->name;
    }
    if(count($path) > $max_levels){
        $abbr_path = array_slice($path,0,$max_levels-1);
        array_push($abbr_path,'…');
        array_push($abbr_path,array_pop($path));
        $path = $abbr_path;
    }
    return implode(' » ',$path);
}


function cdm_taxontree_cache_get($term){
  if(variable_get('cdm_taxontree_cache', 0)){
    return cache_get($term,'cache_cdm_taxontree');
  }
}

function cdm_taxontree_cache_set($term, $child_terms){
  if(variable_get('cdm_taxontree_cache', 0)){
    return cache_set($term,'cache_cdm_taxontree', serialize($child_terms));
  }
}


/**
 * function to empty the cdm_taxontree cache table
 *
 */
function cdm_taxontree_cache_clear() {

    // clear core tables
    $alltables = array('cache_cdm_taxontree');
    foreach ($alltables as $table) {
        cache_clear_all('*', $table, TRUE);
    }
    drupal_set_message('CDM Taxontree Cache cleared.');
    drupal_goto('admin/settings/cdm_dataportal');
}



/**
 * Converts a UUID into a hexadecimal RGB colour code.
 *
 * @param UUID $uuid
 * @return Number hexadecimal
 */
function _uuid_to_rgbhex($uuid){

  $xfoot = _str_crossfoot($uuid);
  $h = $xfoot / 255;
  $h = $h - floor($h);
  $RGB = _hsv_2_rgb($h, 0.45, 1);
  return dechex($RGB['R']).dechex($RGB['G']).dechex($RGB['B']);
}


/**
 * Sums up ASCII values of the character in the given string.
 *
 * @param String $str
 * @return Number
 */
function _str_crossfoot($str){
  $xfoot = 0;
  for($i=0; $i<strlen($str); $i++){
    $xfoot = $xfoot + ord($str[$i]);
  }
  return $xfoot;
}

/**
 * Converts HSV colour codes into their RGB counterpart
 *
 * @param Number $H value 0-1
 * @param Number $S value 0-1
 * @param Number $V value 0-1
 * @return array three values  0-255
 */
function _hsv_2_rgb($H, $S, $V) // HSV Values:Number 0-1
{ //
  $RGB = array();
  
  if($S == 0){
    $R = $G = $B = $V * 255;
  } else {
    $var_H = $H * 6;
    $var_i = floor( $var_H );
    $var_1 = $V * ( 1 - $S );
    $var_2 = $V * ( 1 - $S * ( $var_H - $var_i ) );
    $var_3 = $V * ( 1 - $S * (1 - ( $var_H - $var_i ) ) );
  
    if ($var_i == 0) { $var_R = $V ; $var_G = $var_3 ; $var_B = $var_1 ; }
    else if ($var_i == 1) { $var_R = $var_2 ; $var_G = $V ; $var_B = $var_1 ; }
    else if ($var_i == 2) { $var_R = $var_1 ; $var_G = $V ; $var_B = $var_3 ; }
    else if ($var_i == 3) { $var_R = $var_1 ; $var_G = $var_2 ; $var_B = $V ; }
    else if ($var_i == 4) { $var_R = $var_3 ; $var_G = $var_1 ; $var_B = $V ; }
    else { $var_R = $V ; $var_G = $var_1 ; $var_B = $var_2 ; }
  
    $R = $var_R * 255;
    $G = $var_G * 255;
    $B = $var_B * 255;
  }
  
  $RGB['R'] = $R;
  $RGB['G'] = $G;
  $RGB['B'] = $B;
  
  return $RGB;
}