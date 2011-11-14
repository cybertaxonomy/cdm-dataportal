


  <div class="node<?php if ($sticky&&$page == 0) { print " sticky"; } ?><?php if (!$status) { print " node-unpublished"; } ?> clear-block <?php if ($page == 0) { ?>node-in-list<?php }; ?>">




    <?php if ($page == 0) { ?><h2 class="title"><a href="<?php print $node_url?>"><?php print $title?></a>
	

<?php
// if user can't read comments, we set comment number to 0
$comment_num = $node -> comment ? comment_num_all($node->nid) : 0;
?>

<?php if($comment_num > 0 || $node -> comment != 0 ): ?>
<span class="comment-cloud">
<?php if ($comment_num == 0 && ($node -> comment == 2)): ?>

<a href="<?php echo url('comment/reply/' . $node -> nid, array('fragment' => 'comment-form' ));?>"><img src="<?php echo base_path() . path_to_theme() ?>/images/plus.gif"  alt="+" /></a>

<?php elseif($comment_num != 0): ?>
	<?php echo l($comment_num, 'node/' . $node->nid, array('fragment' => 'comments'));?>
<?php endif; ?>
	</span>&nbsp;
<?php endif; ?>

      



</h2><?php }; ?>
    
	
	<?php if ($page != 0) { ?><div class="supermitted"><span class="submitted"><?php print $submitted?></span></div><?php }; ?>

   
    <div class="content clear-block">




		 
	<?php if ($page != 0) { ?><?php if ($picture) {  print $picture;  }?><?php }; ?>
	
	<?php print $content?></div>



     <?php if ($terms): ?><div class="taxonomy">+ <?php print $terms?></div><?php endif; ?>
 
    <?php if ($links) { ?><div class="links-to-left"><div class="links"><?php print $links?></div></div><?php }; ?>
    <?php if ($page == 0) { ?><div class="submitted-to-right"><span class="submitted"><?php print $submitted?></span></div><?php }; ?> 

  </div>
