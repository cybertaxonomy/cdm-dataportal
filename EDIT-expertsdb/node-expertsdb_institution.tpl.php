<?php

 // pull in the taxon_experts stylesheet
 drupal_add_css(drupal_get_path('module', 'taxon_experts') .'/taxon_experts_view.css');

/* print "<pre>";
print_r($node);
print "</pre>";*/
?>
<?php phptemplate_comment_wrapper(NULL, $node->type); ?>

<div id="node-<?php print $node->nid; ?>" class="node<?php if ($sticky) { print ' sticky'; } ?><?php if (!$status) { print ' node-unpublished'; } ?>">

<?php print $picture ?>

<?php if ($page == 0): ?>
  <h2><a href="<?php print $node_url ?>" title="<?php print $title ?>"><?php print $title ?></a></h2>
<?php endif; ?>

  <?php if ($submitted): ?>
    <span class="submitted"><?php print t('!date — !username', array('!username' => theme('username', $node), '!date' => format_date($node->created))); ?></span>
  <?php endif; ?>

  <div class="content">
    <?php
     print '<h3 class="title-inline">'.t('Institute: ') . $node->field_name[0]['view'] . ' — ' . $node->field_code[0]['view'].'</h3>';

     $output = '';
		 $output .= '<div class="nodeprofile-details">';

     // render complete contact data
     if($node->content['group_contact']){
     	$output .= '<div class="' . $node->content['group_contact']['#attributes']['class'] . '">';
     	$output .= $node->content['field_university']['#value'];
			$output .= $node->content['group_contact']['field_institution_email']['#value'];
			$output .= $node->content['group_contact']['field_institution_phone']['#value'];
			$output .= $node->content['group_contact']['field_institution_url']['#value'];
			$output .= $node->content['group_contact']['field_institution_address']['#value'];
			$output .= '</div>';
     }

     $output .= '</div>';

		 // clear block
		 $output .= '<div class="clear-float"> </div>';

			// configure views
			$views = array(
        'members_by_institute' => array(
        	'title' => t('Members')
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

		 print $output;

      //print $content ?>
  </div>

  <div class="clear-block clear">

    <?php if ($links): ?>
      <div class="links"><?php print $links; ?></div>
    <?php endif; ?>
  </div>

</div>