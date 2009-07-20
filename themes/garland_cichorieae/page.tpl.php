<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language ?>" lang="<?php print $language ?>">
  <head>
    <title><?php print $head_title ?></title>
    <?php print $head ?>
    <?php print $styles ?>
    <style type="text/css" media="all">@import "<?php print base_path() . path_to_theme() ?>/cdm_style.css";</style>
<?php 
   /* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   * OpenLayers.js must be loaded BEFORE jQuery. 
   * If jQuery loaded before $.something will fail in IE8.
   * Therefore we add OpenLayers.js it in the page.tpl.php
   */ 
  print ('<script type="text/javascript" src="'.drupal_get_path('module', 'cdm_dataportal').'/js/OpenLayers/OpenLayers.js'.'"></script>'."\n");
  print $scripts 
?>
<!--    <style type="text/css" media="print">@import "<?php print base_path() . path_to_theme() ?>/print.css";</style>-->
<!--    -->
    <!--[if IE]>
    <style type="text/css" media="all">@import "<?php print base_path() . path_to_theme() ?>/fix-ie.css";</style>
    <![endif]-->
    <!--[if gte IE 7]>
    <style type="text/css" media="all">@import "<?php print base_path() . path_to_theme() ?>/fix-ie7.css";</style>
    <![endif]-->
  </head>
  <body<?php print phptemplate_body_class($sidebar_left, $sidebar_right); ?>>

<!-- Layout -->
  <div id="header-region" class="clear-block"><?php print $header; ?></div>

    <div id="wrapper">
    <div id="container" class="clear-block">

	  <?php /* <div id="upbg"></div> */ ?>

      <div id="header">
      	<div id="edit-logo"></div>
        <div id="logo-floater">
        <?php
          // Prepare header
          $site_fields = array();
          if ($site_name) {
            $site_fields[] = check_plain($site_name);
          }
          if ($site_slogan) {
            $site_fields[] = check_plain($site_slogan);
          }
          $site_title = implode(' ', $site_fields);
          $site_fields[0] = $site_fields[0];
          $site_html = implode(' ', $site_fields);

          if ($logo || $site_title) {
            print '<h1><a href="'. check_url($base_path) .'" title="'. $site_title .'">';
           /* if ($logo) {
              print '<img src="'. check_url($logo) .'" alt="'. $site_title .'" id="logo" />';
            }*/
            print $site_html .'</a></h1>';
          }
        ?>
        </div>
        
        <div id="splash"></div>

		<div id="menu">
          <?php if (isset($primary_links)) : ?>
            <?php print theme('links', $primary_links, array('class' => 'links primary-links')) ?>
          <?php endif; ?>
          <?php if (isset($secondary_links)) : ?>
            <?php print theme('links', $secondary_links, array('class' => 'links secondary-links')) ?>
          <?php endif; ?>
        </div>

      </div> <!-- /header -->

	<div id="contento">

      <?php if ($sidebar_left): ?>
        <div id="sidebar-left" class="sidebar">
          <?php if ($search_box): ?><div class="block block-theme"><?php print $search_box ?></div><?php endif; ?>
          <?php print $sidebar_left ?>
        </div>
      <?php endif; ?>

      <div id="center"><div id="squeeze"><div class="right-corner"><div class="left-corner">
          <?php //if ($breadcrumb): print $breadcrumb; endif; ?>
          <?php if ($mission): print '<div id="mission">'. $mission .'</div>'; endif; ?>

          <?php if ($tabs): print '<div id="tabs-wrapper" class="clear-block">'; endif; ?>
          <?php if ($title): print '<h2'. ($tabs ? ' class="with-tabs"' : '') .'>'. $title .'</h2>'; endif; ?>
          <?php if ($tabs): print '<div class="clear"></div>'.$tabs .'</div>'; endif; ?>

          <?php if (isset($tabs2)): print $tabs2; endif; ?>

          <?php if ($help): print $help; endif; ?>
          <?php if ($messages): print $messages; endif; ?>
          <?php print $content ?>
          <span class="clear"></span>
          <?php print $feed_icons ?>
          <div id="footer"><?php print $footer_message ?></div>
      </div></div></div></div> <!-- /.left-corner, /.right-corner, /#squeeze, /#center -->

      <?php if ($sidebar_right): ?>
        <div id="sidebar-right" class="sidebar">
          <?php if (!$sidebar_left && $search_box): ?><div class="block block-theme"><?php print $search_box ?></div><?php endif; ?>
          <?php print $sidebar_right ?>
        </div>
      <?php endif; ?>
	</div> <!-- contento -->
    </div> <!-- /container -->
  </div>
<!-- /layout -->

  <?php print $closure ?>
  </body>
</html>
