<?php
/**
 * @file
 * pre-drupal8.inc
 *
 * Contains useful functions which will most probably be included into
 * Drupal 8 core and for which did not want to wait that long.
 *
 */

/******************************************************************************
 * description_list from Version: 8.x-dev
 *
 * from issue:
 *   Add a theme_description_list() function to theme a description list
 *   (ex. definition list)
 *   http://drupal.org/node/54898
 *   http://drupal.org/files/drupal8.theme-dl.54.patch
 *****************************************************************************/
/**
 * Preprocesses variables for theme_description_list().
 *
 * "The dl element represents an association list consisting of zero or more
 * name-value groups (a description list). Each group must consist of one or
 * more names (dt elements) followed by one or more values (dd elements).
 * Within a single dl element, there should not be more than one dt element for
 * each name."
 *
 * @see http://html5doctor.com/the-dl-element/
 * @see http://www.w3.org/TR/html-markup/dl.html
 * @see http://www.w3.org/wiki/HTML_lists#Description_lists
 *
 * This means:
 * - The DL may be empty.
 * - If there is a DT, it must be followed by a DD.
 * - There can be multiple DTs followed by one or more DDs.
 * - Each set of DTs and DDs forms a group.
 * - The text of one DT must be unique within the DL.
 * - The DL must contain DT and DD elements only.
 *
 * @param array $variables
 *   An associative array containing:
 *   - groups: A list of groups to render. Each item is an array containing:
 *     - term: One of the following values:
 *       - A string containing the term.
 *       - A list of of strings containing terms.
 *       - A list of render arrays containing elements to render. Each child
 *         element will be wrapped into a DT element. Use #wrapper_attributes
 *         to specify attributes of the DT element.
 *     - description: One of the following values:
 *       - A string containing the description for the term.
 *       - A list of of strings containing descriptions for the term.
 *       - A list of render arrays containing elements to render. Each child
 *         element will be wrapped into a DD element. Use #wrapper_attributes
 *         to specify attributes of the DD element.
 *   - title: (optional) A heading for the list.
 *   - attributes: (optional) Attributes to apply to the DL element.
 *
 * @return void
 *   $variables['groups'] is processed into $variables['items'].
 */
function template_preprocess_description_list(&$variables) {
  $groups = &$variables['groups'];
  $variables['items'] = array();
  $items = &$variables['items'];
  // Loop over all groups and contained terms and descriptions to prepare the
  // simplified 'items' variable for theme_description_list(). But also
  // harmonize all items within 'groups' so subsequently invoked preprocess
  // functions as well as theme_description_list() can operate on a single,
  // reliable structure and syntax.
  foreach ($groups as $group) {
    // DTs always come before DDs, regardless of their order in the array.
    foreach (array('term' => 'dt', 'description' => 'dd') as $key => $tag) {
      $item = &$group[$key];
      // If the item is a string, then there is only one DT/DD.
      if (is_string($item)) {
        $items[] = array(
            'tag' => $tag,
            'attributes' => array(),
            'value' => $item,
        );
        $item = array(array('#prefix' => "<$tag>", '#markup' => $item, '#suffix' => "</$tag>"));
      }
      // Otherwise, there are multiple DTs/DDs.
      else {
        foreach ($item as &$value) {
          if (is_string($value)) {
            $items[] = array(
                'tag' => $tag,
                'attributes' => array(),
                'value' => $value,
            );
            $value = array('#prefix' => "<$tag>", '#markup' => $value, '#suffix' => "</$tag>");
          }
          else {
            $items[] = array(
                'tag' => $tag,
                'attributes' => isset($value['#wrapper_attributes']) ? $value['#wrapper_attributes'] : array(),
                'value' => drupal_render($value),
            );
            $value += array('#prefix' => '', '#suffix' => '');
            $value['#prefix'] = "<$tag>" . $value['#prefix'];
            $value['#suffix'] = $value['#suffix'] . "</$tag>";
          }
        }
      }
    }
  }
}

/**
 * Returns HTML for a description list.
 *
 * @param array $variables
 *   An associative array containing:
 *   - items: The preprocessed list of description list elements to render. Each
 *     item is an associative array containing:
 *     - tag: Either 'dt' or 'dd'.
 *     - attributes: An array of attributes to apply to the tag.
 *     - value: The rendered content for the tag.
 *   - groups: The original list of groups to render. Each item is an array
 *     containing:
 *     - term: One of the following values:
 *       - A string containing the term.
 *       - A list of of strings containing terms.
 *       - A list of render arrays containing elements to render. Each child
 *         element will be wrapped into a DT element. Use #wrapper_attributes
 *         to specify attributes of the DT element.
 *     - description: One of the following values:
 *       - A string containing the description for the term.
 *       - A list of of strings containing descriptions for the term.
 *       - A list of render arrays containing elements to render. Each child
 *         element will be wrapped into a DD element. Use #wrapper_attributes
 *         to specify attributes of the DD element.
 *   - title: (optional) A heading for the list.
 *   - attributes: (optional) Attributes to apply to the DL element.
 *
 * @return string
 *   The rendered description list.
 *
 * @ingroup themeable
 */
function theme_description_list($variables) {
  $items = $variables['items'];
  $output = '';
  foreach ($items as $item) {
    $output .= '<' . $item['tag'] . drupal_attributes($item['attributes']) . '>'; // using drupal_attributes() instead of D8 new Attribute()
    $output .= $item['value'];
    $output .= '</' . $item['tag'] . '>';
  }
  // Only output the list, container, and title, if there are any groups.
  if ($output !== '') {
    $output = '<dl' . drupal_attributes($variables['attributes']) . '>' . $output . '</dl>'; // using drupal_attributes() instead of D8 new Attribute()
    $title = '';
    if (isset($variables['title'])) {
      $title = '<h3>' . $variables['title'] . '</h3>';
    }
    // If not used for styling purposes, this DIV should be an ASIDE.
    $output = '<div class="description_list">' . $title . $output . '</div>';
  }
  return $output;
}

/**
 * Adds one term and one description entry to the given description_list_group $group.
 *
 * Creation of a new entry does not take place if the $description is a string
 * which can be trimmed to an empty string. The $group array will be associating in order to
 * ease aplphabetical ordering of the enties.
 *
 * NOTE: To be used onyl for themable description_list as proposed in
 * http://drupal.org/node/54898 see includes/pre-drupal8.inc
 *
 * @param array $groups
 *  The description_list_groups
 * @param string $term
 *
 * @param string|array $description
 *   a text string or an form element array
 *
 * @param string $suffix
 *   to be appended to the $description if it is not an array
 *
 * @param number $weight
 *   the weight to be used for ordering the TD,DD element groups in the description list
 *
 * @param array $dt_attributes
 *   an array of attributes of the DT element
 *
 */
function _description_list_group_add(&$groups, $term, $description, $suffix = '', $weight=NULL, array $dt_attributes=NULL) {

  if (!is_array($description)) {
    $description = trim(drupal_html_to_text($description));
  }

  if ($description) {
    if (!is_array($description)) {
      $description .= $suffix;
    }

    if(is_array($dt_attributes)) {
      $term_elements = array(
        array(
            '#wrapper_attributes' => $dt_attributes,
            '#markup' => $term
        )
      );
    } else {
      $term_elements = $term;
    }

    $groups[$term] = array(
        'term' => $term_elements,
        'description' => $description,
    );
    if ($weight) {
      $groups[$term]['#weight'] = $weight;
    }
  }
}

/******************************************************************************
 * add an other one below this comment if needed
 *****************************************************************************/