<?php /* $Id: node-nodetype.tpl.php, v 1.0 2007/11/29 quiptime Exp $ edited for own purposes by gaia 2007/12/10 */

if ( $node->type == 'software' && arg(0) == 'node' && is_numeric(arg(1)) && is_null(arg(2)) ) { ?>
  
  <div class="tabs"><ul class="tabs primary">
  <li class="active"><a href="?q=node/<?php print $node->nid; ?>">Summary</a></li>
  <li><a href="?q=node/<?php print $node->nid; ?>/screenshots">Screenshots</a></li>
  <li><a href="?q=node/<?php print $node->nid; ?>/reviews">Reviews</a></li>
  <li><a href="?q=node/<?php print $node->nid; ?>/comments">Comments</a></li>
  </ul>
  </div><br /><br />
  <div >


<div style="border-top:1px solid #444444; border-bottom:1px solid #444444; padding-top:10px; padding-bottom:10px"><?php print content_format('field_icon', $field_icon[0]); ?><br />

date added: <?php if ($submitted): ?>
<span class="submitted"><?php print t('!date — !username', array('!username' => theme('username', $node), '!date' => format_date($node->created))); ?></span>
<?php endif; ?><br />

<?php if (content_format('field_author', $field_author[0]) != '') : ?><label>author:</label>
<?php foreach ($field_author as $author) { ?>
<?php print content_format('field_author', $author) ?>
<?php } ?><br />
<?php endif; ?>

<?php if (content_format('field_version', $field_version[0]) != '') : ?><label>current version:</label>
<?php foreach ($field_version as $version) { ?>
<?php print content_format('field_version', $version) ?>
<?php } ?><br />
<?php endif; ?>	
	
	
<?php if (content_format('field_latest_release_date', $field_latest_release_date[0]) != '') : ?><label>release date:</label> 
<?php foreach ($field_latest_release_date as $release_date) { ?>
<?php print content_format('field_latest_release_date', $release_date) ?>
<?php } ?><br />
<?php endif; ?>
	
<?php if (content_format('field_homepage', $field_homepage[0]) != '') : ?><label>homepage:</label> 
<?php foreach ($field_homepage as $homepage) { ?>
<?php print content_format('field_homepage', $homepage) ?>
<?php } ?><br />
<?php endif; ?>


<label>price:</label>
<?php print $node->field_price[0]['view'] ?>

<?php if (content_format('field_price', $field_price[0]['view']) != '') : ?><label>price:</label> 
<?php foreach ($field_price as $price) { ?>
<?php print $node->field_price[0]['view'] ?>
<?php } ?><br />
<?php endif; ?>
</div >

<?php if (content_format('field_description', $field_description[0]) != '') : ?><div class="field-label">description:</div>
<?php foreach ($field_description as $description) { ?>
<?php print content_format('field_description', $description) ?>
<?php } ?><br />
<?php endif; ?>

<?php if (content_format('field_system_requirements', $field_system_requirements[0]) != '') : ?><label>system requirements:</label><?php foreach ($field_system_requirements as $requirements) { ?>
<?php print content_format('field_system_requirements', $requirements) ?>
<?php } ?><br />
<?php endif; ?>


<?php print $node->content['fivestar_widget']['#value'];?>



<?php /*
  $current_rating = votingapi_get_voting_result('node', $nid, 'percent', 'vote', 'average');
  print theme('fivestar_static', $current_rating->value);*/
?>

<br /><br />

<?php if ($links) { ?><div class="links">&raquo; <?php print $links?></div><?php }; ?>
	
	<br /><br /><br /><br />
  </div>
<?php
}
if ( $node->type == 'software' && arg(0) == 'node' && is_numeric(arg(1)) && arg(2) == 'reviews' ) { ?>
  <div class="tabs"><ul class="tabs primary">
  <li><a href="?q=node/<?php print $node->nid; ?>">Summary</a></li>
  <li><a href="?q=node/<?php print $node->nid; ?>/screenshots">Screenshots</a></li>
  <li class="active"><a href="?q=node/<?php print $node->nid; ?>/reviews">Reviews</a></li>
  <li>
  <?php 
  global $user;
if (in_array('authenticated user', $user->roles)) : 
?><a href="?q=node/add/review/parent/<?php print $node->nid; ?>/<?php 
print $node->field_version[0]['value'];
?>">Create new Review</a><?php 
endif; 
?>
</li>
   <li><a href="?q=node/<?php print $node->nid; ?>/comments">Comments</a></li>
  </ul>
  </div>
  <div class="content"><br /><br />
 
<?php
$view = views_get_view('reviewz');
print views_build_view('embed', $view, array($node->nid), false, 0);
?>
  </div>
<?php
}
if ( $node->type == 'software' && arg(0) == 'node' && is_numeric(arg(1)) && arg(2) == 'screenshots' ) { ?>
  <div class="tabs"><ul class="tabs primary">
  <li><a href="?q=node/<?php print $node->nid; ?>">Summary</a></li>
  <li class="active"><a href="?q=node/<?php print $node->nid; ?>/screenshots">Screenshots</a></li>
  <li><a href="?q=node/<?php print $node->nid; ?>/reviews">Reviews</a></li>
   <li><a href="?q=node/<?php print $node->nid; ?>/comments">Comments</a></li>
  </ul>
  </div>
  <div class="content"><br /><br />
    
	<?php
$view = views_get_view('screenshotz');
print views_build_view('embed', $view, array($node->nid), false, 0);
?>
  </div>

<?php
}
if ( $node->type == 'software' && arg(0) == 'node' && is_numeric(arg(1)) && arg(2) == 'comments' ) { ?>
  <div class="tabs"><ul class="tabs primary">
  <li><a href="?q=node/<?php print $node->nid; ?>">Summary</a></li>
  <li><a href="?q=node/<?php print $node->nid; ?>/screenshots">Screenshots</a></li>
  <li><a href="?q=node/<?php print $node->nid; ?>/reviews">Reviews</a></li>
   <li class="active"><a href="?q=node/<?php print $node->nid; ?>/commentz">Comments</a></li>
  </ul>
  </div>
  <div class="content" ><br /><br />
  <a href="?q=comment/reply/<?php print $node->nid; ?>#comment-form" title="Share your thoughts and opinions related to this posting." class="comment_add">Add new comment</a><br /><br />
<?php
$view = views_get_view('commentz');
print views_build_view('embed', $view, array($node->nid), false, 0);
?>
  </div>
<?php

}

?>