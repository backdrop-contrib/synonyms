
-- SUMMARY --

The Synonyms module extends the Drupal core Taxonomy features. Currently
the module provides this additional functionality:
* support of synonyms
* merging terms one into another (reasonable method against 2 identical terms)
* synonym-friendly autocomplete widget for taxonomy_term_reference fields
* integration with Drupal search functionality enabling searching content by
   synonyms of the terms that the content references

-- REQUIREMENTS --

The Synonyms module requires enabled:
* Taxonomy module
* Text module


-- INSTALLATION --

* Install as usual


-- CONFIGURATION --

* The module itself does not provide any configuration as of the moment.
Although during creation/editing of a Taxonomy vocabulary you will be able
to enable/disable for that particular vocabulary the additional functionality
this module provides.
