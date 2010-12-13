<?php phptemplate_comment_wrapper(NULL, $node->type); ?>

<?php
$node = node_load(arg(1)); 
$ancestors = relativity_load_ancestors($node,1,TRUE);
// sollte nur ein parent sein!
if(isset($ancestors[0])) 
$parentNode = $ancestors[0];
?>


<div id="node-<?php print $node->nid; ?>" class="node<?php if ($sticky) { print ' sticky'; } ?><?php if (!$status) { print ' node-unpublished'; } ?>">

<?php print $picture ?>

<?php if ($page == 0): ?>
  <h2><a href="<?php print $node_url ?>" title="<?php print $title ?>"><?php print $title ?></a></h2>
<?php endif; ?>

<div class="tabs"><ul class="tabs primary">
<li><a href="?q=node/<?php print $parentNode->nid; ?>">Summary</a></li>
<li><a href="?q=node/<?php print $parentNode->nid; ?>/screenshots">Screenshots</a></li>
<li class="active"><a href="?q=node/<?php print $parentNode->nid; ?>/reviews">Reviews</a></li>
<li>
  <?php 
  global $user;
if (in_array('authenticated user', $user->roles)) : 
?><a href="?q=node/add/review/parent/<?php print $parentNode->nid; ?>/<?php 
print $node->field_version[0]['value'];
?>">Create new Review</a><?php 
endif; 
?>
</li>
<li><a href="?q=node/<?php print $parentNode->nid; ?>/comments">Comments</a></li>
</ul>
</div><br /><br />
  
 
  <?php if ($submitted): ?>
    <span class="submitted"><?php print t('!date — !username', array('!username' => theme('username', $node), '!date' => format_date($node->created))); ?></span>
  <?php endif; ?>

  <div class="content">
    <?php print $content ?>
  </div>

  <div class="clear-block clear">
    <div class="meta">
    <?php if ($taxonomy): ?>
      <div class="terms"><?php print $terms ?></div>
    <?php endif;?>
    </div>

    <?php if ($links): ?>
      <div class="links"><?php print $links; ?></div>
    <?php endif; ?>
  </div>

</div>


  
