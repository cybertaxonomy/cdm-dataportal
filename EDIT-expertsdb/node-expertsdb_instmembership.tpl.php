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
    print '<h3 class="title-inline">' . t('Expert:') . ' ' . l($node->field_parent_member[0]['view'],'user/'.$node->uid) . '</h3>';

     // render persons profile
     $profile_node = nodeprofile_load('expertsdb_person', $node->uid);

     // render profile as TEASER to show only the teaser informations
     $profile = node_build_content($profile_node, TRUE, FALSE);

     // create output
     $output = '';
     $output .= '<div class="nodeprofile-person">';

     // render complete contact from nodeprofile
		 $output .= drupal_render($profile->content['group_contact']);

		 $output .= '</div>'; // end of div.person

		 // clear block
 		 $output .= '<div class="clear-block"> </div>';

			// configure views
			$views = array(
        'institutional_memberships_by_ref' => array(
        	'title' => t('Membership')
			),
				'institution_by_node' => array(
					'title' => t('Institution Contact')
			),
			);

			foreach($views as $view_name => $view_settings){
				$view = views_get_view($view_name);
				if(views_access($view)){
					// get view data with institutions node id and user id
					$view_data = views_build_view('block', $view, array($node->field_institution[0]['nid'],$node->uid));
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