<?php
/* $Id; */

/**Camp library for VM
*
* PHP version 4 and 5
*
* LICENSE: This source file is subject to LGPL license
* that is available through the world-wide-web at the following URI:
* http://www.gnu.org/copyleft/lesser.html
*
* @package    Trinity_College_VM_Module
* @author   Chathra Hendehewa <chathra@opensource.lk>
* @author   Mifan Careem <mifan@opensource.lk>
* @copyright  Lanka Software Foundation - http://www.opensource.lk
*/

global $global;

/** Contains library functions for creating forms. */
include_once($global['approot'].'/inc/lib_form.inc');
/** Contains library functions for validating forms. */
include_once($global['approot'].'/inc/lib_validate.inc');
/** Contains library functions for error checking end-user entered data. */
include_once($global['approot'].'/inc/lib_errors.inc');

function _shn_vm_cadd_start($errors=false){
	if($errors)
		display_errors();
	global $global;
	$db = $global['db'];
?>
<h2 align="center"><?php echo _("Register New Project")?></h2>
<?php
	shn_form_fopen(camp);
	shn_form_fsopen(_("General Details"));
	//check for gis and goto mapping page
	include $global['approot']."/conf/config.inc";
	
	if($conf['gis']=='true'){
		shn_form_hidden(array('seq'=>'gis'));
	}
	else{
		shn_form_hidden(array('seq'=>'chk'));
	}
	
	shn_form_text(_("Project Name"),"project_name",'size="50"',array('req'=>true));
	shn_form_opt_select("opt_project_type",_("Project Type"));
?>

<!--<label>Division</label>
<select name="opt_location_type" >-->

<?php
/*
// Display selected Division type only
	$q="select value from config where confkey='division_type' && module_id='cr'";
	//$q ="select option_description from field_options where field_name='cr_pref_div'";
	$ref = $db->Execute($q);
		$val=$ref->fields[0];
	$q ="select loc_uuid,name from location where opt_location_type=$val";
	$ref = $db->Execute($q);
	while(!$ref->EOF){	
		$val=$ref->fields[0];
		$name=$ref->fields[1];
?>
<option value='<?=$val?>'><?=$name?></option>
<?php
		$ref->MoveNext();
	}
*/	
?>

</select><br>
<?php
			
//-----------------------------------------
	shn_form_textarea(_("Address"),"project_address");
	shn_form_fsclose();
//-------------------------
	global $global;
	//include $global['approot']."/inc/lib_location.inc";
	//shn_location_form_org(1,3);
		
//-------------------------
	shn_form_fsopen(_("Contact Person Details"));
	shn_form_text(_("Full Name"),"administrator_contact_name",'size="60"');
	shn_form_text(_("Phone Number"),"administrator_contact_number",'size="60"');
	shn_form_fsclose();
	shn_form_fsopen(_("Project Information"));
	shn_form_text(_("Start date"),"start_date",'size="10"');
	shn_form_text(_("End date"),"end_date",'size="10"');
	shn_form_text(_("Skills needed"),"skills",'size="10"');
	shn_form_fsclose();
	shn_form_submit(_("Next"));
	shn_form_fclose();
}

function _shn_vm_cadd_chk($errors=false){
if($errors)
	display_errors;
	global $global;
	include $global['approot']."/conf/config.inc";
	if($conf['gis']=='false'){
		//add to session if GIS module is not enabled
		$_SESSION['project_name']=$_POST['poject_name'];
		$_SESSION['opt_project_type']=$_POST['opt_project_type'];
		$_SESSION['opt_location_type']=$_POST['opt_location_type'];
		$_SESSION['project_address']=$_POST['project_address'];
		$_SESSION['administrator_contact_name']=$_POST['administrator_contact_name'];
		$_SESSION['administrator_contact_number']=$_POST['administrator_contact_number'];
		$_SESSION['vm_1']=$_POST['1'];
		$_SESSION['vm_2']=$_POST['2'];
		$_SESSION['vm_3']=$_POST['3'];
		$_SESSION['vm_4']=$_POST['4'];
					
	}
	else{
		if((null==$_POST['gps_x']) && null==$_POST['gps_y']){
			$_SESSION['gis_loc_x']=$_POST['loc_x'];
			$_SESSION['gis_loc_y']=$_POST['loc_y'];
		}
		else{
			$_POST['loc_x'] = $_POST['gps_x'];
			$_POST['loc_y'] = $_POST['gps_y'];
			$_SESSION['gis_loc_x']=$_POST['loc_x'];
			$_SESSION['gis_loc_y']=$_POST['loc_y'];
		}
	}
?>
<h2>Project Checklist</h2>
<?php
shn_form_fopen(acmp);
shn_form_fsopen(_("Services / Facilities Available"));
shn_form_hidden(array('seq'=>'commit'));
shn_form_opt_checkbox("opt_camp_service");
shn_form_textarea(_("Other Facilities"),"comments");
shn_form_fsclose();
shn_form_submit(_("Next"));
shn_form_fclose();
}

function _shn_vm_cadd_commit(){
global $global;
//write to database
$_SESSION['opt_camp_service']=$_POST['opt_camp_service'];
$_SESSION['camp_comments']=$_POST['comments'];
foreach($_SESSION['opt_camp_service'] as $a=>$b){
//echo "value is $b";
}
//echo microtime(true);
//var_dump($_SESSION);
_shn_vm_cadd_commit_db();
}

function _shn_vm_create_project_id(){
//create unique camp id
//db->GenID();
$id=time();
return $id;

}

function _shn_vm_sel_map(){
	$_SESSION['project_name']=$_POST['project_name'];
	$_SESSION['opt_project_type']=$_POST['opt_project_type'];
	$_SESSION['opt_location_type']=$_POST['opt_location_type'];
	$_SESSION['project_address']=$_POST['project_address'];
	$_SESSION['administrator_contact_name']=$_POST['administrator_contact_name'];
	$_SESSION['administrator_contact_number']=$_POST['administrator_contact_number'];
	$_SESSION['vm_1']=$_POST['1'];
	$_SESSION['vm_2']=$_POST['2'];
	$_SESSION['vm_3']=$_POST['3'];
	$_SESSION['vm_4']=$_POST['4'];
											
	shn_form_fopen(acmp);
	shn_form_fsopen(_("Location Information"));
	echo _("Plot the Project Location OR Enter its GPS coordinates below");
	shn_form_fsclose();
	shn_form_fsopen(_("Plot Location on Map"));
	shn_form_hidden(array('seq'=>'chk'));
	shn_form_hidden(array('loc_x'=>''));
	shn_form_hidden(array('loc_y'=>''));
	global $global;
	$db=$global['db'];
	include $global['approot']."/conf/config.inc";
	echo "<h2>"._("Area Map")."</h2>";

	//if mod_gis
	if($conf['gis']){
	  include $global['approot']."/mod/gis/conf.inc";
	  if($conf['mod_gis']=='google_maps'){
	  	/*
		 	 * google maps
		 	 *@todo check for online connectivity: exit gracefully
	  	*/
	 		include_once $global['approot']."/mod/gis/plugins/google_maps/handler_google_maps.inc";
			echo _("Enter Camp Location. Click on Map where camp exists");

			$a="select value from config where module_id='gis' and confkey='google_key'";
      $ares=$db->Execute($a);
			$keys=$ares->fields[0];

	
			$w="select value from config where module_id='gis' and confkey='center_x'";
			$wres=$db->Execute($w);
			$wkey=$wres->fields[0];

			$e="select value from config where module_id='gis' and confkey='center_y'";
			$eres=$db->Execute($e);
			$ekey=$eres->fields[0];
																		
			init_map($keys);
			load_map($wkey,$ekey);
			add_marker($_POST['camp_name']);
		}
	}
	else
	{
?>
   <p>No Currently Selected GIS</p> 
<?php
  }
	shn_form_fsclose();
	shn_form_fsopen(_("GPS Coordinates"));
	shn_form_text(_("Northing / Latitude"),"gps_x",'size="60"');
	shn_form_text(_("Easting / Longitude"),"gps_y",'size="60"');
	shn_form_fsclose();
			
	shn_form_submit("Next");
	shn_form_close();
}

function _shn_vm_validate_error(){
	clean_errors();
	$error_flag=false;


	if(null == ($_POST['project_name'])){
			add_error(_("Please Enter the Project Name"));
			$error_flag=true;
	}
/*
        if(!(null == ($_POST['family']))){
		if(!is_numeric(trim($_POST['family']))){
			add_error(_("The Family Count is not a valid number"));
			$error_flag=true;
		}
	}
	if(!(null == ($_POST['total']))){
		if(!is_numeric(trim($_POST['total']))){
			add_error(_("The Total Count is not a valid number"));
			$error_flag=true;
		}
	}
	if(!(null == ($_POST['men']))){
		if(!is_numeric(trim($_POST['men']))){
			add_error(_("The Men Count is not a valid number"));
			$error_flag=true;
		}
	}
	if(!(null == ($_POST['women']))){
		if(!is_numeric(trim($_POST['women']))){
			add_error(_("The Women Count is not a valid number"));
			$error_flag=true;
		}
	}
	if(!(null == ($_POST['children']))){
		if(!is_numeric(trim($_POST['children']))){
			add_error(_("The Children Count is not a valid number"));
			$error_flag=true;
		}
	}
*/	
	return $error_flag;
}

function _shn_vm_get_location_id(){
	if($_SESSION['vm_4']!=null)
		return $_SESSION['vm_4'];
	else if($_SESSION['vm_3']!=null)
		return $_SESSION['vm_3'];
	else if($_SESSION['vm_2']!=null)
		return $_SESSION['vm_2'];
	else 
		return $_SESSION['vm_1'];
}

function _shn_vm_cadd_commit_db(){
//insert to database;
global $global;
$db = $global['db'];

//call unique project id
$proj_id=_shn_vm_create_project_id();

/*--------Phonetic Insertions------------*/
shn_db_insert_phonetic('project_name',$proj_id);
/*---------------------------------------*/

$location_id=_shn_vm_get_location_id();


//enter into project table
$q="insert into project(project_id,name,location_id,opt_camp_type,address) values($proj_id,'{$_SESSION['project_name']}',$location_id,'{$_SESSION['opt_project_type']}','{$_SESSION['project_address']}')";


$res = $db->Execute($q);

//enter into project_reg table
$q="insert into project_reg(project_id,name,orgid,administrator_contact_name,administrator_contact_number) values($proj_id,'{$_SESSION['project_name']}','orgid','{$_SESSION['administrator_contact_name']}','{$_SESSION['administrator_contact_number']}','{$_SESSION['camp_comments']}','{$_SESSION['start_date']}','{$_SESSION['end_date']}','{$_SESSION['skills']}')";

$res = $db->Execute($q);

/*
 * GIS Insertion
*/

global $global;
include $global['approot']."/conf/config.inc";
	

//if mod_gis
if($conf['gis']){
  include $global['approot']."/mod/gis/conf.inc";
  include $global['approot']."/mod/gis/gis_fns.inc";
	shn_gis_dbinsert($proj_id,$location_id,$conf['mod_gis'],$_SESSION['gis_loc_x'],$_SESSION['gis_loc_y'],NULL);
}
				



/*insert services into camp_services table
 *currently add checked values.
*/								
	foreach($_SESSION['opt_project_service'] as $a => $b){
	//currently add checked values.
		$q = "insert into project_services(project_id,opt_project_service,value) values($proj_id,'{$b}',1)";
	$res = $db->Execute($q);
	}
?>
<h2>You Have Succesfully Registered "<?=$_SESSION['name']?>"</h2>

<br>
<br>
<h3><?php echo _("The Availability of Services & Facilities for the Project are as follows:")?></h3>
<!--<p>Facilities / Services Availability</p>-->
<ul>

<!--To have the results shown in table format-->
<div id ="result">
<table>
        <thead>
        		<td><?php echo _("Service Type")?></td>
            <td><?php echo _("Availability")?></td>
        </thead>
        
<tbody>
<tr>
<?php
$a="select option_code,option_description from field_options where field_name='opt_project_service'";
$ref=$db->Execute($a);
while(!$ref->EOF){
	$val=$ref->fields[0];
	$name=$ref->fields[1];
	$flag=false;
	foreach($_SESSION['opt_project_service'] as $a => $b){
		if($val==$b){
					
    		echo "<td>$name</td>";
    		echo "<td>". _("Available"). "</td>";
					
			//echo "<li>$name :  Available </li><br>";
			$flag=true;			
		}
	}
	if(!$flag){
		echo "<td>$name</td>";
		echo "<td>"._("NOT AVAILABLE")."</td>";
		//echo "<li class=\"fontspecial\">$name : NOT AVAILABLE </li><br>";
	}
	?>
	</tr>
	<?php
	$ref->MoveNext();
}

?>
</tbody>
</table>
</div>

</ul>
<br><br>
<p><?php echo _("Use the Left Navigation Menu to Continue.")?></p>

<?php	
}
?>
