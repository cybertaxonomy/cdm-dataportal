<?php
/*
   $variables = array(
    'content'        => ($teaser && $node->teaser) ? $node->teaser : $node->body,
    'date'           => format_date($node->created),
    'links'          => $node->links ? theme('links', $node->links, array('class' => 'links inline')) : '',
    'name'           => theme('username', $node),
    'node'           => $node,  // we pass the actual node to allow more customization
    'node_url'       => url('node/'. $node->nid),
    'page'           => $page,
    'taxonomy'       => $taxonomy,
    'teaser'         => $teaser,
    'terms'          => theme('links', $taxonomy, array('class' => 'links inline')),
    'title'          => check_plain($node->title)
  );
*/

// pull in the stylesheet
drupal_add_css(drupal_get_path('module', 'taxon_experts') .'/taxon_experts_view.css');

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
     print '<h3 class="title-inline">' . t('Expert:') . ' ' . l($node->field_parent_person[0]['view'],'user/'.$node->uid) . '</h3>';

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

			print $output;

      $display_options =  array(
            //taxa
            8 => array('term_path'=> true, 'notes'=>$node->field_taxon_notes[0]['view']),
            //geo
            3 => array('term_path'=> true, 'notes'=>$node->field_geo_notes[0]['view']),
            // methods
            2 => array('term_path'=> true),
          );

      // prepare complete taxonomy from taxonomy and cdm_taxontree fields
      $taxonomy = array();
      foreach($node->field_expertise as $taxa){
      	$term = taxonomy_get_term($taxa['tid']);
      	$taxonomy[$term->tid] = $term;
      }
      foreach($node->field_georegions as $taxa){
      	$term = taxonomy_get_term($taxa['tid']);
      	$taxonomy[$term->tid] = $term;
      }

      $taxonomy = array_merge($taxonomy,$node->taxonomy);
      // try to access taxonInterests; persons privacy is used from persons profile_node
      $taxon_interests =  theme('expertdb_interest_categories', $taxonomy, $display_options, true, $profile_node);
      
	  if(!empty($taxon_interests)){
		  print '<div class="expertsdb-taxon-interests">';
	      print $taxon_interests;
	      print '</div>';
	  }

      $view_args = array('');

      if($node->field_taxon_project){
        foreach($node->field_taxon_project as $project){
          $view_args[0] .= (strlen($view_args[0]) ? '+' : '').$project['nid'];
        }
      }

      $projects_view = views_get_view('projects_by_interest');
      if(views_access($projects_view)){
        $out = views_build_view('block', $projects_view, $view_args, false, false);
        if($out){
          print '<h3 class="title">'.t('Projects:').'</h3>'.$out;
        } else {
          // empty table but add_link at bottom
          print '<h3 class="title">'.t('Projects:').'</h3>'.theme('views_view_table_projects_by_interest',$projects_view, array());
        }
      }

            if(!empty($node->field_taxon_notes[0]['value'])){
				print '<div class="expertsdb-taxon-notes">';
				print '<h3 class="title">' . t('Taxon Notes:') . '</h3>';
      	print $node->field_taxon_notes[0]['view'];
      	print '</div>';
      }

      if(!empty($node->field_geo_notes[0]['value'])){
				print '<div class="expertsdb-geo-notes">';
				print '<h3 class="title">' . t('Geo Notes:') . '</h3>';
	      print $node->field_geo_notes[0]['view'];
	      print '</div>';
      }
      //print $content ?>
  </div>

  <div class="clear-block clear">

    <?php if ($links): ?>
      <div class="links"><?php print $links; ?></div>
    <?php endif; ?>
  </div>

</div>