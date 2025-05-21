package rdno_core

import (
	denv "github.com/jurgen-kluft/ccode/denv"
	cunittest "github.com/jurgen-kluft/cunittest/package"
)

// rdno_core is the core package for Arduino projects.
//
// It can compile for Desktop (Windows, Mac, Linux) but it mocks many of the
// systems that are available on the Arduino platform.
// For example:
// - GPIO
// - Serial
// - Wire
// - I2C
// - UART
// - WiFi

// GetPackage returns the package object of 'rdno_core'
func GetPackage() *denv.Package {
	// Dependencies
	unittestpkg := cunittest.GetPackage()

	// The main (rdno_core) package
	mainpkg := denv.NewPackage("rdno_core")
	mainpkg.AddPackage(unittestpkg)

	// 'rdno_core' library
	mainlib := denv.SetupCppLibProject("rdno_core", "github.com\\jurgen-kluft\\rdno_core")

	// 'rdno_core' unittest project, only for Windows, Mac and Linux
	maintest := denv.SetupCppTestProjectForDesktop("rdno_core"+"test", "github.com\\jurgen-kluft\\rdno_core")
	maintest.AddDependencies(mainlib)
	maintest.AddDependencies(unittestpkg.GetMainLib()...)

	mainpkg.AddMainLib(mainlib)
	mainpkg.AddUnittest(maintest)
	return mainpkg
}
