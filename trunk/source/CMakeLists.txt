#set lib name here!
set(LIB_NAME "ConstructionSim")

set(PHYSX_DIR $ENV{PHYSX_HOME}  CACHE PATH "PhysX folder")

#msvc only, need other check if gcc
if(${CMAKE_CL_64})
	set(PHYSX_BIN_DIR "${PHYSX_DIR}/bin/win64")
	set(DLL_SUFFIX "_x64")
else()
	set(PHYSX_BIN_DIR "${PHYSX_DIR}/bin/win32")
	set(DLL_SUFFIX "_x86")
endif()


#Set include directories
include_directories(${GASS_COMMON_INCLUDE_DIRS} "${PROJECT_SOURCE_DIR}")

set(CPP_FILES "main.cpp")
set(H_FILES "")

#Add some folders where to search for dependency libraries
link_directories(${GASS_COMMON_LIB_DIRS})

add_executable (${LIB_NAME} ${CPP_FILES} ${H_FILES})
set_target_properties(${LIB_NAME} PROPERTIES DEBUG_POSTFIX _d)

#Add project dependency libs
target_link_libraries(${LIB_NAME} ${GASS_COMMON_LINK_LIBRARIES})

if(EXISTS "${GASS_DEPENDENCIES_DIR}/OpenSceneGraph-v3.0.1")
	set(OSG_DIR  "${GASS_DEPENDENCIES_DIR}/OpenSceneGraph-v3.0.1" CACHE PATH "OSG folder")
else()
	set(OSG_DIR $ENV{OGRE_HOME} CACHE PATH "OSG folder")
endif()

if(EXISTS "${GASS_DEPENDENCIES_DIR}/ogre-v1-9")
	set(OGRE_DIR  "${GASS_DEPENDENCIES_DIR}/ogre-v1-9" CACHE PATH "Ogre 3D folder")
else()
	set(OGRE_DIR $ENV{OGRE_HOME} CACHE PATH "Ogre 3D folder")
endif()


	set(DEP_DLL_FILES_RELEASE 
	${GASS_DEPENDENCIES_DIR}/lib/tbb.dll 
	${GASS_DEPENDENCIES_DIR}/lib/OpenAL32.dll
	${GASS_DEPENDENCIES_DIR}/lib/OIS.dll
	#${GASS_DEPENDENCIES_DIR}/MyGUI_3.2.0/bin/release/MyGUIEngine.dll
	${OGRE_DIR}/bin/release/OgreMain.dll
	${OGRE_DIR}/bin/release/OgreTerrain.dll
	${OGRE_DIR}/bin/release/OgrePaging.dll
	${OGRE_DIR}/bin/release/OgreOverlay.dll
	${OGRE_DIR}/bin/release/Plugin_CgProgramManager.dll
	${OGRE_DIR}/bin/release/OgreOverlay.dll
	${OGRE_DIR}/bin/release/Plugin_OctreeSceneManager.dll
	${OGRE_DIR}/bin/release/Plugin_ParticleFX.dll
	${OGRE_DIR}/bin/release/RenderSystem_Direct3D9.dll
	${OGRE_DIR}/bin/release/RenderSystem_GL.dll
	${OGRE_DIR}/bin/release/cg.dll
	${OSG_DIR}/bin/zlib1.dll
	${OSG_DIR}/bin/libpng13.dll
	${OSG_DIR}/bin/glut32.dll
	${OSG_DIR}/bin/ot12-OpenThreads.dll
	${OSG_DIR}/bin/osg80-osg.dll
	${OSG_DIR}/bin/osg80-osgUtil.dll
	${OSG_DIR}/bin/osg80-osgDB.dll
	${OSG_DIR}/bin/osg80-osgGA.dll
	${OSG_DIR}/bin/osg80-osgText.dll
	${OSG_DIR}/bin/osg80-osgViewer.dll
	${OSG_DIR}/bin/osg80-osgTerrain.dll
	${OSG_DIR}/bin/osg80-osgFX.dll
	${OSG_DIR}/bin/osg80-osgManipulator.dll
	${OSG_DIR}/bin/osg80-osgVolume.dll
	${OSG_DIR}/bin/osg80-osgPresentation.dll
	${OSG_DIR}/bin/osg80-osgSim.dll
	${OSG_DIR}/bin/osg80-osgAnimation.dll
	${OSG_DIR}/bin/osg80-osgWidget.dll
	${OSG_DIR}/bin/osg80-osgParticle.dll
	${OSG_DIR}/bin/osg80-osgShadow.dll
	
	${PHYSX_BIN_DIR}/PhysX3Common${DLL_SUFFIX}.dll
	${PHYSX_BIN_DIR}/PhysX3${DLL_SUFFIX}.dll
	${PHYSX_BIN_DIR}/PhysX3Cooking${DLL_SUFFIX}.dll
	${PHYSX_BIN_DIR}/PhysX3CharacterKinematic${DLL_SUFFIX}.dll
	${PHYSX_BIN_DIR}/PhysX3Profile${DLL_SUFFIX}.dll
	
	
	${GASS_DIR}/bin/release/GASSCore.dll
	${GASS_DIR}/bin/release/GASSSim.dll 
	${GASS_DIR}/bin/release/GASSPluginOgre.dll
	${GASS_DIR}/bin/release/GASSPluginOIS.dll
	${GASS_DIR}/bin/release/GASSPluginODE.dll
	${GASS_DIR}/bin/release/GASSPluginPagedGeometry.dll
	${GASS_DIR}/bin/release/GASSPluginEnvironment.dll
	${GASS_DIR}/bin/release/GASSPluginOpenAL.dll
	${GASS_DIR}/bin/release/GASSPluginGame.dll
	${GASS_DIR}/bin/release/GASSPluginBase.dll
	${GASS_DIR}/bin/release/GASSPluginRakNet.dll
	
	)
	set(DEP_DLL_FILES_DEBUG 
	${GASS_DEPENDENCIES_DIR}/lib/tbb_debug.dll
	${GASS_DEPENDENCIES_DIR}/lib/OpenAL32.dll	
	${GASS_DEPENDENCIES_DIR}/lib/OIS_d.dll
	#${GASS_DEPENDENCIES_DIR}/MyGUI_3.2.0/bin/debug/MyGUIEngine_d.dll
	${OGRE_DIR}/bin/debug/OgreMain_d.dll
	${OGRE_DIR}/bin/debug/OgreTerrain_d.dll
	${OGRE_DIR}/bin/debug/OgrePaging_d.dll
	${OGRE_DIR}/bin/debug/OgreOverlay_d.dll
	${OGRE_DIR}/bin/debug/Plugin_CgProgramManager_d.dll
	${OGRE_DIR}/bin/debug/OgreOverlay_d.dll
	${OGRE_DIR}/bin/debug/Plugin_OctreeSceneManager_d.dll
	${OGRE_DIR}/bin/debug/Plugin_ParticleFX_d.dll
	${OGRE_DIR}/bin/debug/RenderSystem_Direct3D9_d.dll
	${OGRE_DIR}/bin/debug/RenderSystem_GL_d.dll
	${OGRE_DIR}/bin/release/cg.dll
	${OSG_DIR}/bin/glut32D.dll
	${OSG_DIR}/bin/libpng13d.dll
	${OSG_DIR}/bin/osg80-osgAnimationd.dll
	${OSG_DIR}/bin/osg80-osgd.dll
	${OSG_DIR}/bin/osg80-osgDBd.dll
	${OSG_DIR}/bin/osg80-osgFXd.dll
	${OSG_DIR}/bin/osg80-osgGAd.dll
	${OSG_DIR}/bin/osg80-osgManipulatord.dll
	${OSG_DIR}/bin/osg80-osgParticled.dll
	${OSG_DIR}/bin/osg80-osgPresentationd.dll
	${OSG_DIR}/bin/osg80-osgShadowd.dll
	${OSG_DIR}/bin/osg80-osgSimd.dll
	${OSG_DIR}/bin/osg80-osgTerraind.dll
	${OSG_DIR}/bin/osg80-osgTextd.dll
	${OSG_DIR}/bin/osg80-osgUtild.dll
	${OSG_DIR}/bin/osg80-osgViewerd.dll
	${OSG_DIR}/bin/osg80-osgVolumed.dll
	${OSG_DIR}/bin/osg80-osgWidgetd.dll
	${OSG_DIR}/bin/ot12-OpenThreadsd.dll
	${OSG_DIR}/bin/zlib1d.dll
	${PHYSX_BIN_DIR}/PhysX3CommonCHECKED${DLL_SUFFIX}.dll
	${PHYSX_BIN_DIR}/PhysX3CHECKED${DLL_SUFFIX}.dll
	${PHYSX_BIN_DIR}/PhysX3CookingCHECKED${DLL_SUFFIX}.dll
	${PHYSX_BIN_DIR}/PhysX3CharacterKinematicCHECKED${DLL_SUFFIX}.dll
	${PHYSX_BIN_DIR}/PhysX3Profile${DLL_SUFFIX}.dll
	${GASS_DIR}/bin/debug/GASSCore_d.dll
	${GASS_DIR}/bin/debug/GASSSim_d.dll 
	${GASS_DIR}/bin/debug/GASSPluginOgre_d.dll
	${GASS_DIR}/bin/debug/GASSPluginOSG_d.dll
	${GASS_DIR}/bin/debug/GASSPluginOIS_d.dll
	${GASS_DIR}/bin/debug/GASSPluginODE_d.dll
	${GASS_DIR}/bin/debug/GASSPluginPhysx3_d.dll
	${GASS_DIR}/bin/debug/GASSPluginPagedGeometry_d.dll
	${GASS_DIR}/bin/debug/GASSPluginEnvironment_d.dll
	${GASS_DIR}/bin/debug/GASSPluginOpenAL_d.dll
	${GASS_DIR}/bin/debug/GASSPluginGame_d.dll
	${GASS_DIR}/bin/debug/GASSPluginBase_d.dll
	${GASS_DIR}/bin/debug/GASSPluginRakNet_d.dll
	)

#add dll files to out dir to enable debugging from IDE without install step
foreach(DLL_FILE ${DEP_DLL_FILES_DEBUG})
	ADD_CUSTOM_COMMAND(TARGET ${LIB_NAME}
			POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_if_different ${DLL_FILE} $<TARGET_FILE_DIR:${LIB_NAME}>)
endforeach()

foreach(DLL_FILE ${DEP_DLL_FILES_RELEASE})
	ADD_CUSTOM_COMMAND(TARGET ${LIB_NAME}
			POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_if_different ${DLL_FILE} $<TARGET_FILE_DIR:${LIB_NAME}>)
endforeach()
			
ADD_CUSTOM_COMMAND(TARGET ${LIB_NAME}
			POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/GASS.xml $<TARGET_FILE_DIR:${LIB_NAME}>
			COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/data
			COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/../data ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/data)

#install exe			

install(TARGETS ${LIB_NAME}  RUNTIME DESTINATION ${CSIM_INSTALL_BIN_DIR_DEBUG} CONFIGURATIONS Debug)
install(TARGETS ${LIB_NAME}  RUNTIME DESTINATION ${CSIM_INSTALL_BIN_DIR_RELEASE} CONFIGURATIONS Release)

#install config
#FILE(GLOB GASS_CONFIG "${CMAKE_CURRENT_SOURCE_DIR}/bin/Configuration/*.xml")
#install(FILES ${GASS_CONFIG} DESTINATION ${GASS_INSTALL_BIN_DIR_DEBUG})
#install(FILES ${GASS_CONFIG} DESTINATION ${GASS_INSTALL_BIN_DIR_RELEASE})
install(FILES  ${CMAKE_CURRENT_SOURCE_DIR}/GASS.xml" DESTINATION ${CSIM_INSTALL_BIN_DIR_DEBUG})
install(FILES  ${CMAKE_CURRENT_SOURCE_DIR}/GASS.xml" DESTINATION ${CSIM_INSTALL_BIN_DIR_RELEASE})

#install data
install(DIRECTORY ../data DESTINATION ${CSIM_INSTALL_BIN_DIR} FILES_MATCHING PATTERN "*.*" PATTERN ".svn" EXCLUDE)

#install dlls
install (FILES  ${DEP_DLL_FILES_DEBUG} DESTINATION ${CSIM_INSTALL_BIN_DIR_DEBUG} CONFIGURATIONS Debug)
install (FILES  ${DEP_DLL_FILES_RELEASE} DESTINATION ${CSIM_INSTALL_BIN_DIR_DEBUG} CONFIGURATIONS Release)


