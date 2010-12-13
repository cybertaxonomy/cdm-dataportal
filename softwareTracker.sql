
#
# Table structure for table 'basar_software-tracker_node'
#

CREATE TABLE `basar_software_tracker_node` (
  `nid` int(11) NOT NULL,
  `software_title` varchar(255) NOT NULL,
  `author` varchar(255) default NULL,
  `home_url` varchar(255) default NULL,
  `download_url` datetime default NULL,
  `lastReviewed` varchar(50) default NULL,
  `version` varchar(50) default NULL,
  `licence` varchar(50) default NULL,
  `price` varchar(255) default NULL,
  `screenshot` varchar(50) default NULL,
  `icon` varchar(255) default NULL,
  `description` text,
  `os_platform` varchar(50) default NULL,
  `system_requirements` text,
  `interface` varchar(255) default NULL,
  PRIMARY KEY  (`nid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1