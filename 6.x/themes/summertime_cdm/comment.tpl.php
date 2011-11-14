<?php
// $Id: comment.tpl.php,v 1.1 2009/08/25 19:15:43 troy Exp $
?>

<div class="clear-block comment<?php if ($comment->status == COMMENT_NOT_PUBLISHED) print ' comment-unpublished'; ?><?php if ($new != '') { ?> new <?php } ?>">
<?php echo theme("vote_up_down_widget", $comment->cid, 'comment'); ?>

<?php if ($picture) { print $picture; } ?>
<span class="submitted"><?php print $submitted; ?> &nbsp;<span class="this-link 	"><?php echo "<a href=\"$node_url#comment-$comment->cid\">#</a>"; ?></span></span>


<div class="content"><?php print $content; ?></div>
<div class="links-comment "><?php print $links; ?></div> &nbsp;</div>
