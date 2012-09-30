<?php 
/**
 * @file
 * Custom theme implementation to display the 'Request new password' link below 
 * the 'Log in' button.
 *
 * @author w.addink <w.addink@eti.uva.nl>
 */
?>
<div id="<?php print $block_html_id; ?>" class="<?php print $classes; ?>"<?php print $attributes; ?>>

  <?php print render($title_prefix); ?>
<?php if ($block->subject): ?>
  <h2<?php print $title_attributes; ?>><?php print $block->subject ?></h2>
<?php endif;?>
  <?php print render($title_suffix); ?>

  <div class="content"<?php print $content_attributes; ?>>

<?php

  $elements = drupal_get_form('user_login_block');

 /*
  *  drupal_render seems to add html to the elements array
  *  and instead of printing what is returned from drupal_render
  * you can use the added html in ['#children'] elements of the arrays
  * to build the form in the order you want.
  */
  $rendered = drupal_render($elements);

  $output  = '<form action="' . $elements['#action'] .
                            '" method="' . $elements['#method'] .
                            '" id="' . $elements['#id'] .
                            '" accept-charset="UTF-8"><div>';

  $output .= $elements['name']['#children'];
  $output .= $elements['pass']['#children'];
  $output .= $elements['form_build_id']['#children'];
  $output .= $elements['form_id']['#children'];
  $output .= $elements['actions']['#children'];
  $output .= $elements['links']['#children'];
  $output .= '</div></form>';
  print $output;
?>
  </div>
</div>