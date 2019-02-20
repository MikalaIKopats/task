<?php

include('db_site.inc');
include ('db_section.inc');
// clear database
/*
$site = new db_site();
$site->delete_where();

$section = new db_section();
$section->delete_where();
*/
$site1_data = array( 'description' => 'Site Description',
    'site_url' => 'http://as.by',
    'active' => 0);

$section1_data = array( 'description' => 'Section Description Section Description',
    'url' => 'http://as.by/des/Section Description',
    'site_id' => '4');

// add
/*
$site1 = new db_site();
print_r($site1_data);
$site1_data = $site1->filter_insert_data($site1_data);
$site1->insert($site1_data);
$site1->load();
print_r($site1);

$section1 = new db_section();

print_r($section1_data);
$section1_data = $section1->filter_data($section1_data);
print_r($section1_data);
$section1_data = $section1->filter_insert_data($section1_data);
print_r($section1_data);
$section1->insert($section1_data);
$section1->load();
print_r($section1);
*/

//Update
/*
$site2_data = array( 'id' => 4,
    'description' => 'Site Description Description',
    'site_url' => 'http://as.by',
    'active' => 1);
$site2 = new db_site();
$site2_data = $site2->filter_update_data($site2_data);
print_r($site2_data);
$site2->update($site2_data);
print_r($site2);
*/

$section2_data = array( 'id' =>'4',
    'description' => 'Section Description Section_4',
    'url' => 'http://as.by/des',
    'site_id' => '4');
$section2 = new db_section();
$section2_data = $section2->filter_update_data($section2_data);
print_r($section1_data);
$section2->update($section2_data);
print_r($section2);



$section1 = new db_section();
print_r($section1_data);

print_r($section1->check_double_data($section1_data));
print_r($section1);

$section1_data = array( 'description' => 'Section Description Section Description',
    'url' => 'http://as.by/des/Section Description',
    'site_id' =>'ds');
print_r($section1->check_type_data($section1_data));
print_r($section1);