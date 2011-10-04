<?php
	//$ID$
	/** 
	 * displays table of files that have been downloaded.
	 **/
	function file_download_form_admin_overview($for_export = false,$alt=false){
		$rows = array();
		if($downloads = file_download_form_user_downloads()){
			//we want to organize by e-mail addresses
			$temp = array();
			$sort = $alt ? 'filepath':'email';
	
			foreach($downloads as $d){
				if(!isset($temp[$d[$sort]])) $temp[$d[$sort]] = array();
				$temp[$d[$sort]][] = $d;
			}
			$downloads = $temp;
			unset($temp);

			foreach($downloads as $key=>$dls){
				foreach($dls as $x=>$d){
					$row = array();
					//email
					if($alt){
						$row[] = ($x==0 ? array_pop(explode('/',$key)):'');
						$row[] = $d['email'];
					}else{
						$row[] = ($x==0 ? $key:'');
						$row[] = array_pop(explode('/',$d['filepath']));
					}
					$row[] = $d['download_count'];
					$row[] = $d['last_download'];
					$rows[] = $row;
				}
			}
		}else{
			$rows[] = array(array('data'=>t('No files have been downloaded'),'colspan'=>4));
			$downloads = false;
		}
		
		if($alt) $headers = array(t('File'),t('Email'),t('# of downloads'),t('last download'));
		else	 $headers = array(t('Email'),t('File'),t('# of downloads'),t('last download'));
		
		if($for_export){
			if($downloads){
				array_unshift($rows,$headers);
				return $rows;
			}
			return false;
		}
		return theme('table',$headers,$rows);
	}
	
	
	/**
	 * Triggers alternative view of download table
	 **/
	function file_download_form_admin_overview_alt(){
		return file_download_form_admin_overview(false,true);
	}
	
	/**
	 * Exports table of downloads as a csv file
	 **/
	function file_download_form_admin_export($alt=false){
		//get list of user downloads
		if($downloads = file_download_form_admin_overview(true,$alt)){
			//build csv
			drupal_set_header("Content-type: text/csv");
			drupal_set_header("Content-Disposition: attachment; filename=file_downloads_by_".($alt ? 'files':'emails').'_'.date('m-d-Y').".csv");
			foreach($downloads as &$row) $row = implode(",",$row);
			$downloads = implode("\n",$downloads);
			echo $downloads;
		//else suck it up and just display the table
		}else return file_download_form_admin_overview();
	}
	
	/**
	 * Exports alternatve view of downloads table 
	 **/
	function file_download_form_admin_export_alt(){
		return file_download_form_admin_export(true);
	}
?>