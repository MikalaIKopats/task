--
-- Table structure for table `section`
--

DROP TABLE IF EXISTS `section`;
CREATE TABLE `section` (
  `id` int(5) unsigned NOT NULL auto_increment,
  `description` varchar(50) NOT NULL default '',
  `site_id` int(10) unsigned NOT NULL default '0',
  `active` tinyint(4) NOT NULL default '0',
  `code` varchar(32) default NULL,
  `mpaa_rating` varchar(16) NOT NULL default 'Unknown',
  `inventory_type_id` int(4) NOT NULL default '1',
  `url` varchar(255) NOT NULL default '',
  `beacon_conversion_id` int(11) default NULL,
  `log_predict_stats` tinyint(1) NOT NULL default '0',
  `predict_version` tinyint(1) unsigned NOT NULL default '0',
  `is_click_fraud` tinyint(1) NOT NULL default '0',
  PRIMARY KEY  (`id`),
  UNIQUE KEY `unique_name_site_id` (`description`,`site_id`),
  UNIQUE KEY `unique_code` (`site_id`,`code`),
  KEY `ix_section` (`site_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;


--
-- Table structure for table `site`
--

DROP TABLE IF EXISTS `site`;
CREATE TABLE `site` (
  `id` int(5) unsigned NOT NULL auto_increment,
  `description` varchar(50) NOT NULL default '',
  `active` tinyint(4) NOT NULL default '0',
  `default_section_id` int(5) unsigned NOT NULL default '0',
  `uses_section_codes` tinyint(3) unsigned NOT NULL default '0',
  `site_url` varchar(255) NOT NULL default '',
  `code` varchar(32) default NULL,
  `sections_share_channels` tinyint(1) NOT NULL default '1',
  `vurl_id` int(10) NOT NULL default '0',
  `allow_site_to_vurl_lookup` tinyint(1) NOT NULL default '0',
  PRIMARY KEY  (`id`),
  UNIQUE KEY `description` (`description`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
