<?php 
    foreach ($items as $item) { 
      $user_profile = node_load($item['nid']);
      $output = l($user_profile->title, 'user/'.$user_profile->uid); 
      print $output ?><br/>
<?php } ?>
