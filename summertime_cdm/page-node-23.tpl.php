<?php
// $Id: page.tpl.php,v 1.3 2009/09/14 19:25:45 troy Exp $
?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"    "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd"> 
<html xmlns="http://www.w3.org/1999/xhtml" lang="<?php echo $language->language ?>" xml:lang="<?php echo $language->language ?>" dir="<?php echo $language->dir ?>" id="html-main">

<head>
  <title><?php echo $head_title ?></title>
  <?php echo $head ?>
  <?php echo $styles ?>
  <?php echo $scripts ?>
  <script type="text/javascript"><?php /* Needed to avoid Flash of Unstyle Content in IE */ ?> </script>

<!--[if lte IE 6]>
<link rel="stylesheet" href="<?php echo base_path() . path_to_theme() ?>/ie6-fixes.css" type="text/css">
<![endif]-->

<!--[if lte IE 7]>
<link rel="stylesheet" href="<?php echo base_path() . path_to_theme() ?>/ie7-fixes.css" type="text/css">
<![endif]-->



</head>

<body class="body-main">
    <div class="body-background">
    <div class="border-top">
     
	 
	 <!-- balloons--><div id="circules">



      <!--make-it-center--><div class="make-it-center">




       <img class="blick" src="<?php echo base_path() . path_to_theme() ?>/images/blick.gif"  alt="" />

      <div class="cap"></div>

      <!--main-content--><div class="main-content">


               <div class="top clear-block">



            <!-- log-box-->
			
			<div class="log-box <?php if ($_GET['q'] == 'user/register') echo 'inversed'; ?>">
                
				 
				 
    <?php if (!$user->uid): ?>
				 <!-- log-box-first --><div class="log-box-first"><div class="left-side"><div class="right-side">
				 <?php echo l(t("Log in"), "user");?>
				 <!-- /log-box-first --></div></div></div>


				 <?php if ($registration_enabled): ?>    	
                 <!-- log-box-second --><div class="log-box-second"><div class="left-side"><div class="right-side">
				 <?php echo l(t("Create new account"), "user/register");?>
				 <!-- /log-box-second --></div></div></div>
				 <?php endif; ?>

	
 	
    <?php else: ?> 

				<!-- log-box-first --><div class="log-box-first"><div class="left-side"><div class="right-side">
				<?php echo t("<strong>!user</strong>", array('!user' => l($user->name, "user"))); ?>&nbsp;|&nbsp;<?php echo l(t("Edit"), "user/" . $user->uid . "/edit");?>
				 <!-- /log-box-first --></div></div></div>


				 <!-- log-box-second --><div class="log-box-second"><div class="left-side"><div class="right-side">
				 <?php echo l(t("Log out"), "logout"); ?>
				 <!-- /log-box-second --></div></div></div>

    <?php endif; ?>



            <!-- /log-box--></div>


<div class="rss-box">
    
	<?php if ($feed_icons): ?>
	<a href="<?php echo url("rss.xml"); ?>"><img src="<?php echo base_path() . path_to_theme() ?>/images/rss-bg.gif"  alt="RSS" /></a>
	<?php endif; ?>



 
<a class="home" href="<?php print $base_path ?>" title="<?php print t('Home') ?>"></a>
<?php if (module_exists('contact')): ?>
<a class="mail" href="<?php echo url('contact'); ?>"></a>
<?php endif; ?>
                     </div>

                     </div>



 <?php if ($top_menu): ?>



                 <div class="menu-background">
                 <div class="main-menu clear-block">
                 <div class="menu-side-left"><div class="menu-side-right clear-block">


 
<?php echo $top_menu; ?> 

					
					
					</div><!--menu-side-right-->
					</div> <!--menu-side-left-->
					</div>
					</div>

<?php endif; ?> 


                   <div class="banner"><div class="banner-text"><h2><a href="<?php echo $front_page; ?>" title="<?php echo t('Home') ?>"><?php echo $site_name ?></a></h2>
				   

				    <?php if ($site_slogan): ?>
					<p><?php echo $site_slogan; ?></p>
					<?php endif; ?> 
					</div></div>


<?php if ($left): ?>


                   <!--column-left--> <div class="column-left">
                  <?php echo $left; ?>
		           <!-- /column-left--></div>

<?php endif; ?>


<!-- column-2 --><div class="column-2
<?php if (!$right && !$left): ?>no-right-and-left-columns
<?php elseif (!$left): ?>
no-left-column
<?php elseif (!$right): ?>
no-right-column
<?php endif; ?>
">


                   <div class="right-col-main clear-block">


                   <!--column-center--> <div class="column-center clear-block">
                   <div class="col-cent-border">




<?php if ($show_messages): ?>
<?php echo $messages; ?>
<?php endif; ?>

<?php echo $help ?>

<?php if ($content_top): ?><div id="content-top" class="clear-block"><?php echo $content_top ?></div><?php endif; ?>

 
<?php if ($tabs): ?><div class="tabs"><?php echo $tabs; ?></div><?php endif; ?>
<?php 
$output = '<div class="pageroute pageroute_individual</div>';
echo $output
?>  

<?php if ($title): ?><h1 class="page-title"><?php echo $title ?></h1><?php endif; ?>
		
		

<?php echo $content; ?>



<?php if ($content_bottom): ?><div id="content-top" class="clear-block"><?php echo $content_bottom ?></div><?php endif; ?>


                   <!-- /col-cent-border--></div>
                   <!-- /column-center-->  </div>



<?php if ($right): ?>
                   <!--column-right--><div class="column-right clear-block">
           
<?php echo $right; ?>


                   <!-- /column-right--></div>

<?php endif; ?> 



                   <!-- /column-right-main--></div>
                    <!-- footer -->
                    <div class="footer clear-block">


 <?php echo $footer ?>


<div class="clear-both">
  <?php echo $footer_message ?>
  <?php echo $closure ?>
</div>

<!-- /footer--></div>
         <!-- /main-content--></div>



      <!-- /make-it-center--> </div>
      <!-- /balloons--></div>      </div>


</div></div>
</body>
</html>
