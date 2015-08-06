from WMCore.Configuration import Configuration
config = Configuration()

config.section_("General")
config.General.requestName = 'RecHitStudyRAWEnergy_MCMinimumBias_CUETP8M1_100'
config.General.transferOutputs = True
config.General.transferLogs = True

config.section_("JobType")
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'Conf_RecHitStudy.py'
config.JobType.allowUndistributedCMSSW = False
config.JobType.outputFiles=['HcalRecHitSpectra.root']

config.section_("Data")
config.Data.inputDataset = '/MinBias_TuneCUETP8M1_13TeV-pythia8/RunIISpring15DR74-NoPURecodebug_MCRUN2_74_V8B-v1/GEN-SIM-RECODEBUG'
config.Data.inputDBS = 'https://cmsweb.cern.ch/dbs/prod/global/DBSReader/'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 5
config.Data.publication = False
config.Data.totalUnits = 100

config.Data.ignoreLocality = False

config.section_("Site")
config.Site.storageSite = 'T3_US_FNALLPC'

