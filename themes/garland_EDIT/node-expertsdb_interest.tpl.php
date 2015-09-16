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
      print '<h3>'.t('Expert: ').$node->content['field_parent_person']['#value'].'</h3>';
      
      $display_options =  array(
            //taxa
            12 => array('term_path'=> true, 'notes'=>$node->field_taxon_notes[0]['view']),
            //geo
            13 => array('term_path'=> true, 'notes'=>$node->field_geo_notes[0]['view']),
            // methods
            5 => array('term_path'=> true),
          );
          
      
      print theme('expertdb_interest_categories', $node->taxonomy, $display_options, true);
      
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
          print '<h3>'.t('Projects:').'</h3>'.$out;
        } else {
          // empty table but add_link at bottom
          print '<h3>'.t('Projects:').'</h3>'.theme('views_view_table_projects_by_interest',$projects_view, array());
        }
      }
      //print $content ?>
  </div>

  <div class="clear-block clear">

    <?php if ($links): ?>
      <div class="links"><?php print $links; ?></div>
    <?php endif; ?>
  </div>

</div>