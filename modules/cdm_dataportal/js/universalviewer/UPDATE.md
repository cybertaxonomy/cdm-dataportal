
1. setup a separate project (e.g. in  `~/workspace/universalviewer`) for UV. 
   Follow the instructions given in [https://dev.e-taxonomy.eu/redmine/issues/5083#universalviewer](https://dev.e-taxonomy.eu/redmine/issues/5083#universalviewer) below *setting up universalviewer dev environment*
2. copy the contents of universalviewer `~/workspace/universalviewer/dist/` to 
   this directory (`cdm-dataportal/modules/cdm_dataportal/js/universalviewer/uv/`), e.g.:
   
~~~
cd ~/workspaces/libs/universalviewer
grunt build
rm -rf ~/workspaces/cdm/cdm-dataportal/modules/cdm_dataportal/js/universalviewer/uv
cp -a ~/workspaces/libs/universalviewer/dist ~/workspaces/cdm/cdm-dataportal/modules/cdm_dataportal/js/universalviewer/uv
rm ~/workspaces/cdm/cdm-dataportal/modules/cdm_dataportal/js/universalviewer/uv/uv.zip
~~~

## Debugging:

Universalviewer code is minified, for debugging, build the universalviewer project with the `--dev` option:

~~~
grunt build --dev  
~~~

You also can temporarily replace the folder `modules/cdm_dataportal/js/universalviewer/uv` by a symlink to the univeralviewer 
projekt, so "instant deployment" of the uv code to the dataportal project.
