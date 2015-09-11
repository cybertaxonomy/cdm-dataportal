<?php
/**
 * @file
 * Custom theme implementation to display a Drupal page.
 *
 * See modules/system/page.tpl.php for more information about available
 * variables.
 */
?>
<?php print render($page['header']); ?>
  <div id="wrapper">
    <div id="container" class="clearfix">

      <div id="header">
        <div id="edit-logo"></div>
        <div id="logo-floater">
        
        <?php if ($site_title): ?>

            <h1 id="branding"><a href="<?php print $front_page ?>" title="<?php print $site_name_and_slogan ?>">
            <?php print $site_title ?>
            </a></h1>

        <?php endif; ?>
        </div>
        <div id="splash"></div>
        <?php if ($primary_nav): print $primary_nav; ?>
        <?php else: ?><div id="menu"> </div><?php endif; ?>
        <?php if ($secondary_nav): print $secondary_nav; endif; ?>
      </div> <!-- /#header -->
      <div id="contento">
      <?php if ($page['sidebar_first']): ?>
        <div id="sidebar-first" class="sidebar">

          <?php if(!empty($page['search'])): ?>
            <div class="block block-theme">
            <?php print render($page['search']); ?>
            </div>
         <?php endif; ?>

          <?php print render($page['sidebar_first']); ?>
          
        </div>
      <?php endif; ?>

      <div id="center"><div id="squeeze"><div class="right-corner"><div class="left-corner">
          <?php // print $breadcrumb; ?>
          <?php if ($page['highlighted']): ?><div id="highlighted"><?php print render($page['highlighted']); ?></div><?php endif; ?>
          <a id="main-content"></a>
          <?php // Unfortunately in D7 tabs are always set, even if there are none.. ?>
          <?php if ($tabs = render($tabs)): ?><div id="tabs-wrapper" class="clearfix"><?php endif; ?>
          <?php print render($title_prefix); ?>
          <?php if ($title): ?>
            <h2<?php if($tabs = render($tabs)):?><?php print ' class="with-tabs"' ; ?><?php endif; ?>><?php print $title ?></h2>
          <?php endif; ?>
          <?php print render($title_suffix); ?>
          <?php if ($tabs = render($tabs)): ?><?php print render($tabs); ?></div><?php endif; ?>
          <?php print render($tabs2); ?>
          <?php print render($page['help']); ?>
          <?php print $messages; ?>
          
          <?php if ($action_links): ?><ul class="action-links"><?php print render($action_links); ?></ul><?php endif; ?>
          <div class="clearfix">
            <?php print render($page['content']); ?>
          </div>
          <?php print $feed_icons ?>
          <?php print render($page['footer']); ?>
      </div></div></div></div> <!-- /.left-corner, /.right-corner, /#squeeze, /#center -->

      <?php if ($page['sidebar_second']): ?>
        <div id="sidebar-second" class="sidebar">
          <?php print render($page['sidebar_second']); ?>
        </div>
      <?php endif; ?>
    </div> <!-- contento -->
    </div> <!-- /#container -->
  </div> <!-- /#wrapper --> 
