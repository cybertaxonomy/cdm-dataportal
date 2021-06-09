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
    $form['link_templates'][$id]['configure'] = array('#type' => 'link', '#title' => t('configure'), '#href' => 'admin/config/content/link_templates/' . $id);
    $form['link_templates'][$id]['disable'] = array('#type' => 'link', '#title' => t('disable'), '#href' => 'admin/config/content/link_templates/' . $id . '/disable', '#access' => !$form['link_templates'][$id]['#is_fallback']);
    $form['link_templates'][$id]['weight'] = array(
      '#type' => 'weight',
      '#title' => t('Weight for @title', array('@title' => $link_template->name)),
      '#title_display' => 'invisible',
      '#default_value' => $link_template->weight,
    );
  }
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Save changes'));
  return $form;
}

/**
 * Form submission handler for filter_admin_overview().
 */
function ext_links_admin_overview_submit($form, &$form_state) {
  foreach ($form_state['values']['formats'] as $id => $data) {
    if (is_array($data) && isset($data['weight'])) {
      // Only update if this is a form element with weight.
      db_update('filter_format')
        ->fields(array('weight' => $data['weight']))
        ->condition('format', $id)
        ->execute();
    }
  }
  filter_formats_reset();
  drupal_set_message(t('The text format ordering has been saved.'));
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
  $form = $variables['form'];

  $rows = array();
  foreach (element_children($form['link_templates']) as $id) {
    $form['link_templates'][$id]['weight']['#attributes']['class'] = array('text-format-order-weight');
    $rows[] = array(
      'data' => array(
        drupal_render($form['link_templates'][$id]['title']),
        drupal_render($form['link_templates'][$id]['link']),
        drupal_render($form['link_templates'][$id]['glue']),
        drupal_render($form['link_templates'][$id]['category']),
        drupal_render($form['link_templates'][$id]['weight']),
        drupal_render($form['link_templates'][$id]['configure']),
        drupal_render($form['link_templates'][$id]['disable']),
      ),
      'class' => array('draggable'),
    );
  }
  $header = array(t('Title'), t('Link'), t('Glue'), t('Weight'), t('Category'), array('data' => t('Operations'), 'colspan' => 2));
  $output = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'text-format-order')));
  $output .= drupal_render_children($form);

  drupal_add_tabledrag('text-format-order', 'order', 'sibling', 'text-format-order-weight');

  return $output;
}
