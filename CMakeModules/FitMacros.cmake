macro(checkDependencies appName dependencies)
	set(missingDep)
	foreach(dep ${dependencies})
	  if(NOT TARGET ${dep})
		set(missingDep ${missingDep} " " ${dep})
	  endif()
	endforeach(dep)

	if(missingDep)
		message("Project ${appName} is missing dependencies:" ${missingDep})
	endif()

endmacro()