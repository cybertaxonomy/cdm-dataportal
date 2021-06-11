<?php

/**
 * @file
 * Administrative page callbacks for the External Links module.
 */

/**
 * Page callback: Form constructor for a form to list and reorder external link templates.
 *
 * @ingroup forms
 * @see ext_links_menu()
 * @see ext_links_admin_overview_submit()
 */
function ext_links_admin_overview($form) {

  $link_templates = ext_links_templates();

  $form['#tree'] = TRUE;
  foreach ($link_templates as $id => $link_template) {
    $form['link_templates'][$id]['title'] = array('#markup' => check_plain($link_template->title));
    $form['link_templates'][$id]['link'] = array('#markup' => check_plain($link_template->link));
    $form['link_templates'][$id]['glue'] = array('#markup' => check_plain($link_template->glue));
    $form['link_templates'][$id]['category'] = array('#markup' => check_plain($link_template->category));
    $form['link_templates'][$id]['configure'] = array('#type' => 'link', '#title' => t('configure'), '#href' => 'admin/config/cdm_dataportal/ext_links/' . $link_template->id);
    $form['link_templates'][$id]['status'] = array('#type' => 'link',
      '#title' => $link_template->status == 1 ? t('disable') : t('enable'),
      '#href' => 'admin/config/cdm_dataportal/ext_links/' . $link_template->id . '/status/' . ($link_template->status == 1 ? '0' : '1'));
    $form['link_templates'][$id]['weight'] = array(
      '#type' => 'weight',
      '#title' => t('Weight for @title', array('@title' => $link_template->id)),
      '#title_display' => 'invisible',
      '#default_value' => $link_template->weight,
    );
    $form['link_templates'][$id]['#attributes']['class'] = [];
    if($link_template->status != 1){
      $form['link_templates'][$id]['#attributes']['class'][] = 'disabled';
    }
  }
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Save changes'));
  return $form;
}

/**
 * Form submission handler for ext_links_admin_overview().
 */
function ext_links_admin_overview_submit($form, &$form_state) {
  foreach ($form_state['values']['link_templates'] as $id => $data) {
    if (is_array($data) && isset($data['weight'])) {
      // Only update if this is a form element with weight.
      db_update('ext_links')
        ->fields(array('weight' => $data['weight']))
        ->condition('id', $id)
        ->execute();
    }
  }
  filter_formats_reset();
  drupal_set_message(t('The text format ordering has been saved.'));
}

/**
 * Page callback: Displays external link template add/edit form.
 *
 * @param object|null $link_template
 *   (optional) An object representing an external link template, with the following properties:
 *   - title: The link title
 *   - link: The link url template.
 *   - glue: The string to concatenate name parts in the URL query string.
 *   - status: (optional) An integer indicating whether the ext link is
 *     enabled (1) or not (0). Defaults to 1.
 *   - weight: (optional) The weight of the external link, which controls its
 *     placement in external link block. If omitted, the weight is set to 0.
 *     Defaults to NULL.
 *
 * @return
 *   A form array.
 *
 * @see filter_menu()
 */
function ext_links_admin_link_template_page($link_template = NULL) {
  if (!isset($link_template->id)) {
    drupal_set_title(t('Add external link'));
    $link_template = (object) array(
      'title' => NULL,
      'link' => 'https://',
      'glue' => '',
      'weight' => 0,
      'status' => 1
    );
  }
  return drupal_get_form('ext_links_admin_link_template_form', $link_template);
}

function ext_links_admin_appearance_page(){

  drupal_set_title(t('Configure the appearance of external links'));

  $form['ext_links_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('External links appearance options'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE
  );
  $form['ext_links_options']['ext_links_appearance_grouped'] = [
    '#type' => 'checkbox',
    '#title' => t('Show external links grouped by category with headings.'),
    '#default_value' => variable_get('ext_links_appearance_grouped', 1)
  ];

  return system_settings_form($form);
}


/**
 * Form constructor for the External Link add/edit form.
 *
 * @param $link_template
 *   A link template object having the properties:
 *   - id: A machine-readable name representing the ID of the external link.
 *     If this corresponds to an existing external link, that format will be
 *     updated; otherwise, a new external link will be created.
 *   - title: The link title
 *   - link: The link url template.
 *   - glue: The string to concatenate name parts in the URL query string.
 *   - status: (optional) An integer indicating whether the ext link is
 *     enabled (1) or not (0). Defaults to 1.
 *   - weight: (optional) The weight of the external link, which controls its
 *     placement in external link block. If omitted, the weight is set to 0.
 *     Defaults to NULL.
 *
 * @see ext_links_admin_link_template_form_validate()
 * @see ext_links_admin_link_template_form_submit()
 * @ingroup forms
 */
function ext_links_admin_link_template_form($form, &$form_state, $link_template) {

  $form['#link_template'] = $link_template;
  $form['#tree'] = TRUE;

  $form['id'] = array(
    '#type' => 'machine_name',
    '#required' => TRUE,
    '#default_value' => $link_template->id,
    '#maxlength' => 255,
    '#machine_name' => array(
      'exists' => 'ext_link_exists',
    ),
    '#disabled' => !empty($link_template->id),
  );
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => $link_template->title,
    '#description' => t('The text of the link that will be displayed.'),
    '#required' => TRUE,
  );
  $form['link'] = array(
    '#type' => 'textfield',
    '#title' => t('Link'),
    '#description' => t('The URL of the link to the external resource or service. 
    The species name parts will be appended to this string.<p>E.g. 
    <code>https://external-service.test/images?q=</code> will result in 
    <code>https://external-service.test/images?q=Bellis+perennis</code> when the glue string is set to "+"</p>'),
    '#default_value' => $link_template->link,
    '#required' => TRUE,
  );
  $form['glue'] = array(
    '#type' => 'textfield',
    '#title' => t('Glue'),
    '#description' => t('The string used to concatenate genus and species name parts in the query string.'),
    '#default_value' => $link_template->glue,
    '#required' => false,
  );
  $form['status'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enabled'),
    '#default_value' => $link_template->status
  );
  $form['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#default_value' => $link_template->weight,
    '#delta' => 10,
  );
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Save'));

  return $form;
}

/**
 * Form submission handler for ext_links_admin_link_template_form().
 */
function ext_links_admin_link_template_form_submit($form, &$form_state) {
  // Remove unnecessary values.
  form_state_values_clean($form_state);

  // Add the submitted form values to the text link_template, and save it.
  $link_template = $form['#link_template'];
  foreach ($form_state['values'] as $key => $value) {
    $link_template->$key = $value;
  }
  $status = ext_links_save($link_template);

  switch ($status) {
    case SAVED_NEW:
      drupal_set_message(t('Added external link %link_template.', array('%link_template' => $link_template->id)));
      break;

    case SAVED_UPDATED:
      drupal_set_message(t('The text link_template %link_template has been updated.', array('%link_template' => $link_template->id)));
      break;
  }

  drupal_goto('admin/config/cdm_dataportal/ext_links');
}

/**
 * Sets the status of a link_template.
 *
 * Used as page callback in menu items.
 *
 * @param $link_template
 *    The link template
 * @param $status
 *    The status value to set (values: 1 or 0)
 */
function ext_links_admin_link_template_set_status($link_template, $status){
    $link_template->status = $status == 1 ? 1 : 0;
    ext_links_save($link_template);
    drupal_goto("admin/config/cdm_dataportal/ext_links");
}

/**
 * Returns HTML for the text format administration overview form.
 *
 * @param $variables
 *   An associative array containing:
 *   - form: A render element representing the form.
 *
 * @ingroup themeable
 */
function theme_ext_links_admin_overview($variables) {

  drupal_add_css(drupal_get_path('module', 'ext_links'). '/ext-links.admin.css');

  $form = $variables['form'];

  $rows = array();
  foreach (element_children($form['link_templates']) as $id) {
    $row_class_attributes = $form['link_templates'][$id]['#attributes']['class'];
    $row_class_attributes[] = 'draggable';
    $form['link_templates'][$id]['weight']['#attributes']['class'] = array('text-format-order-weight');
    $rows[] = array(
      'data' => array(
        drupal_render($form['link_templates'][$id]['title']),
        drupal_render($form['link_templates'][$id]['link']),
        drupal_render($form['link_templates'][$id]['glue']),
        drupal_render($form['link_templates'][$id]['category']),
        drupal_render($form['link_templates'][$id]['weight']),
        drupal_render($form['link_templates'][$id]['configure']),
        drupal_render($form['link_templates'][$id]['status']),
      ),
      'class' => $row_class_attributes,
    );
  }
  $header = array(t('Title'), t('Link'), t('Glue'), t('Category'), t('Weight'), array('data' => t('Operations'), 'colspan' => 2));
  $output = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'link-templates-order')));
  $output .= drupal_render_children($form);

  drupal_add_tabledrag('link-templates-order', 'order', 'sibling', 'text-format-order-weight', null, null, TRUE);

  return $output;
}
