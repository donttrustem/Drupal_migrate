<?php

/**
 * @file perla.inc
 * XML import from La Perla.
 */

/**
 * XML import from La Perla Class.
 */
class LaPerlaxmlimport extends XMLMigration {
  public function __construct($arguments) {
    parent::__construct($arguments);
    $this->description = t('XML feed of La Perla Properties.');

    // There isn't a consistent way to automatically identify appropriate
    // "fields" from an XML feed, so we pass an explicit list of source fields.

    $source_fields = array(
      'reference' => t('Ref'),
      'location' => t('Location'),
      'title' => t('Title'),
      'price' => t('Price'),
      'category' => t('Status'),
      'propertyType' => t('Property Type'),
      'bedrooms' => t('Bedrooms'),
      'bathrooms' => t('Bathrooms'),
      'floorSize' => t('Dwelling Size'),
      'plotSize' => t('Plot Size'),
      'description' => t('Description'),
      'image' => t('Property Photos'),
    );

    $items_url = 'http://xml2u.com/Xml/La%20Perla%20estates_1426/4244_Default.xml';
    $item_xpath = '/document/Clients/Client/properties/Property';
    $item_ID_xpath = 'propertyid';

    $items_class = new MigrateItemsXML($items_url, $item_xpath, $item_ID_xpath);
    $this->source = new MigrateSourceMultiItems($items_class, $source_fields);
    $this->destination = new MigrateDestinationNode('property');
dd($this->destination);
    // The source ID here is the one retrieved from the XML listing URL, and
    // used to identify the specific item's URL.
    // $sourceid = '/document/FileDetails/orderName/';
    // $sourceid = 'myspace';
    $source_key = array(
      'bar' => array(
        'type' => 'varchar',
        'length' => 255,
        'not null' => TRUE,
		'description' => 'Title',
      ),
    );
    $this->map = new MigrateSQLMap($this->machineName, $source_key, MigrateDestinationRole::getKeySchema());

    // TIP: Note that for XML sources, in addition to the source field passed to
    // addFieldMapping (the name under which it will be saved in the data row
    // passed through the migration process) we specify the Xpath used to retrieve
    // the value from the XML.
    //$this->addFieldMapping('title', 'title_remote')
     // ->defaultValue('Foobar');

     //$this->addFieldMapping('field_ref', 'field_ref_remote')
     //  ->xpath('/document/Clients/Client/properties/Property/reference');

    // $this->addFieldMapping('field_location_prop', 'field_location_prop_remote')
    //   ->xpath('/document/Clients/Client/properties/Property/Address/location');

    $this->addFieldMapping('title', 'title')
      ->xpath('title');

    // $this->addFieldMapping('field_price', 'field_price_remote')
    //   ->xpath('/document/Clients/Client/properties/Property/Address/Price/price');

    // $this->addFieldMapping('field_status', 'field_status_remote')
    //   ->xpath('/document/Clients/Client/properties/Property/category');

    // $this->addFieldMapping('field_listing_type', 'field_listing_remote')
    //   ->xpath('/document/Clients/Client/properties/Property/Description/propertyType');

    // $this->addFieldMapping('field_cee_epc', 'field_cee_epc_remote')
    //   ->xpath('/document/Clients/Client/properties/Property/EPC');

    // $this->addFieldMapping('field_bedrooms', 'field_bedrooms_remote')
    //   ->xpath('/document/Clients/Client/properties/Property/Description/bedrooms');

    // $this->addFieldMapping('field_bathrooms', 'field_bathrooms_remote')
    //   ->xpath('/document/Clients/Client/properties/Property/Description/fullBathrooms');

    // $this->addFieldMapping('field_sq_ft_', 'field_sq_ft_remote')
    //   ->xpath('/document/Clients/Client/properties/Property/Description/FloorSize/floorSize');

    // $this->addFieldMapping('field_lot_size', 'field_lot_size_remote')
    //   ->xpath('/document/Clients/Client/properties/Property/Description/PlotSize/plotSize');

    // $this->addFieldMapping('field_interior_features', 'field_interior_features_remote')
    //   ->xpath('/document/Clients/Client/properties/Property/Description/Features');

    // $this->addFieldMapping('body', 'body_remote')
    //   ->xpath('/document/Clients/Client/properties/Property/Description/description');

    // $this->addFieldMapping('body_remote:format', 'filtered_html');

    // $this->addFieldMapping('field_image:file_replace')
    //   ->defaultValue(MigrateFile::FILE_EXISTS_REUSE); //FILE_EXISTS_REUSE is in the MigrateFile class


    // $this->addFieldMapping('field_image', 'field_image_remote:source_dir')
    //   ->xpath('/document/Clients/Client/properties/Property/images/image/image');

    // $this->addFieldMapping('field_image:format', 'filtered_html');

    $unmapped_sources = array();
    $this->addUnmigratedSources($unmapped_sources);

    // Declare unmapped destination fields.
    $unmapped_destinations = array(
      'revision_uid',
      'changed',
      'status',
      'promote',
      'sticky',
      'revision',
      'log',
      'language',
      'tnid',
      'is_new',
      'body:language',
    );
  }

  /**
   * {@inheritdoc}
   */
  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }

    $ctype = (string)$row->XML->Type;

    // Set variable for return code.
    $ret = FALSE;
  }
}