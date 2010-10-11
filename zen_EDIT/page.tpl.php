<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="<?php print $language ?>" xml:lang="<?php print $language ?>">

<head>
  <title><?php print $head_title; ?></title>
  <?php print $head; ?>
  <?php print $styles; ?>
  <?php print $scripts; ?>
</head>

<?php /* different ids allow for separate theming of the home page */ ?>
<body class="<?php print $body_classes; ?>">
  <div id="page">
    <div id="header">
      <div id="logo-title">
      
        <?php print $search_box; ?>      
        <?php if ($logo): ?>
          <a href="<?php print $base_path; ?>" title="<?php print t('Home'); ?>">
            <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" id="logo" />
          </a>
        <?php endif; ?>
        
        <div id="name-and-slogan">
        
        <?php if ($site_name): ?>
          <h1 id='site-name'>
            <a href="<?php print $base_path ?>" title="<?php print t('Home'); ?>">
              <?php print $site_name; ?>
            </a>
          </h1>
        <?php endif; ?>
        
        <?php if ($site_slogan): ?>
          <div id='site-slogan'>
            <?php print $site_slogan; ?>
          </div>
        <?php endif; ?>
        
        </div> <!-- /name-and-slogan -->
        
      </div> <!-- /logo-title -->
     
      
      <div id="navigation" class="menu <?php if ($primary_links) { print "withprimary"; } if ($secondary_links) { print " withsecondary"; } ?> ">
          <?php if ($primary_links): ?>
            <div id="primary" class="clear-block">
              <?php print theme('menu_links', $primary_links); ?>
            </div>
          <?php endif; ?>
          
          <?php if ($secondary_links): ?>
            <div id="secondary" class="clear-block">
              <?php print theme('menu_links', $secondary_links); ?>
            </div>
          <?php endif; ?>
      </div> <!-- /navigation -->
      
      <?php if ($header || $breadcrumb): ?>
        <div id="header-region">
          <?php print $breadcrumb; ?>
          <?php print $header; ?>
        </div>
      <?php endif; ?>
      
    </div> <!-- /header -->

    <div id="container" class="clear-block">
                  
      <?php if ($sidebar_left): ?>
        <div id="sidebar-left" class="column sidebar">
          <?php print $sidebar_left; ?>
        </div> <!-- /sidebar-left -->
      <?php endif; ?>  
    
      <div id="main" class="column"><div id="squeeze">
        <?php if ($mission): ?><div id="mission"><?php print $mission; ?></div><?php endif; ?>
        <?php if ($content_top):?><div id="content-top"><?php print $content_top; ?></div><?php endif; ?>
        <?php if ($title): ?><h1 class="title"><?php print $title; ?></h1><?php endif; ?>
        <?php if ($tabs): ?><div class="tabs"><?php print $tabs; ?></div><?php endif; ?>
        <?php print $help; ?>
        <?php print $messages; ?>
        <?php print $content; ?>
        <?php print $feed_icons; ?>
        <?php if ($content_bottom): ?><div id="content-bottom"><?php print $content_bottom; ?></div><?php endif; ?>
      </div></div> <!-- /squeeze /main -->

      <?php if ($sidebar_right): ?>
        <div id="sidebar-right" class="column sidebar">
          <?php print $sidebar_right; ?>
        </div> <!-- /sidebar-right -->
      <?php endif; ?>

    </div> <!-- /container -->

    <div id="footer-wrapper">
      <div id="footer">
        <?php print $footer_message; ?>
      </div> <!-- /footer -->
    </div> <!-- /footer-wrapper -->
    
    <?php print $closure; ?>
    
  </div> <!-- /page -->

</body>
</html>