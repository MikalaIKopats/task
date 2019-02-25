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

/*Секция 1*/

$site1 = new db_site();
$site1_data = array( 'description' => 'Site Description',
                     'site_url' => 'http://as.by',
                     'active' => 0);
print_r($site1_data);

$site1_data = $site1->filter_insert_data($site1_data);
$site1->insert($site1_data);
$site1->load();
print_r($site1);

/*Секция 2.1*/
/*
$section1_data = array( 'description' => 'Section Description Section Description',
    'url' => 'http://as.by/des/Section Description',
    'site_id' => '1');

$section1 = new db_section();

print_r($section1_data);
$section1_data = $section1->filter_data($section1_data);
print_r($section1_data);
*/

/*Секция 2.2*/
/*
$section1_data = array( 'description' => 'Section Description Section Description',
    'url' => 'http://as.by/des/Section Description',
    'site_id' => '9');
$section1 = new db_section();
print_r($section1_data);
$section1_data = $section1->filter_data($section1_data);
print_r($section1_data);
$section1_data = $section1->filter_insert_data($section1_data);
print_r($section1_data);
$section1->insert($section1_data);
$section1->load();
print_r($section1);

$section2_data = array( 'id' =>'5',
    'description' => 'Section Description Section_4',
    'url' => 'http://as.by/des',
    'site_id' => '9');
$section2 = new db_section();
$section2_data = $section2->filter_update_data($section2_data);
print_r($section1_data);
$section2->update($section2_data);
print_r($section2);
*/

/*Секция 3*/
/*
$section1_data = array(
    'description' => 'Section Description Section_4',
    'url' => 'http://as.by/des');
$section1 = new db_section();
print_r($section1_data);

print_r($section1->check_double_data($section1_data));
print_r($section1);
*/

/*Секция 4*/
/*
$section1_data = array( 'description' => 120,
    'url' => 'http://as.by/des/Section Description',
    'site_id' =>'fdsfds');
$section1 = new db_section();
print_r($section1->check_type_data($section1_data));
print_r($section1);
*/