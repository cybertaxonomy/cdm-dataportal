<div class="comment<?php if ($comment->status == COMMENT_NOT_PUBLISHED) print ' comment-unpublished'; ?>">
<?php if ($new != '') { ?><span class="new"><?php print $new; ?></span><?php } ?>
<h3 class="title"><?php print $title; ?></h3>
<?php if ($picture) print $picture; ?>
    <div class="submitted"><?php print t('On ') . format_date($comment->timestamp, 'custom', 'F jS, Y'); ?> <?php print theme('username', $comment) . t(' says:'); ?></div>
    <div class="content"><?php print $content; ?></div>
    <div class="links"><?php print $links; ?></div>
</div>
