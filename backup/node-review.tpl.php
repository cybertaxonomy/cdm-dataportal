<?php
$node = node_load(arg(1)); 
$ancestors = relativity_load_ancestors($node,1,TRUE);
// sollte nur ein parent sein!
if(isset($ancestors[0])){ 
$parentNode = $ancestors[0];
?>
 <div class="tabs"><ul class="tabs primary">
 <li><a href="?q=node/<?php print $parentNode->nid; ?>">Summary</a></li>
  <li><a href="?q=node/<?php print $parentNode->nid; ?>/screenshots">Screenshots</a></li>
  <li class="active"><a href="?q=node/<?php print $parentNode->nid; ?>/reviews">Reviews</a></li>
  <li><a href="?q=node/<?php print $parentNode->nid; ?>/comments">Comments</a></li>
</ul>
  </div><br /><br />
  
  <?php if (content_format('field_interfaces__data_standard', $field_interfaces__data_standard[0]) != '') : ?><label interfaces & data standard:</label>
<?php foreach ($field_interfaces__data_standard as $interfaces__data_standard) { ?>
<?php print content_format('field_interfaces__data_standard', $interfaces__data_standard) ?>
<?php } ?><br />
<?php endif; ?>

<?php if (content_format('field_review', $field_review[0]) != '') : ?><label>current version:</label>
<?php foreach ($field_review as $review) { ?>
<?php print content_format('field_review', $review) ?>
<?php } ?><br />
<?php endif; ?>	
	
	
<?php if (content_format('field_version_reviewed_2', $field_version_reviewed_2[0]) != '') : ?><label>version reviewed_2:</label> 
<?php foreach ($field_version_reviewed_2 as $version_reviewed_2) { ?>
<?php print content_format('field_version_reviewed_2', $version_reviewed_2) ?>
<?php } ?><br />
<?php endif; ?>
<?php

}

?>