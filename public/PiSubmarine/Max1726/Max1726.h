#pragma once

#include "PiSubmarine/RegUtils.h"
#include "PiSubmarine/Max1726/Units.h"
#include "PiSubmarine/Api/Internal/I2C/DriverConcept.h"
#include <bitset>
#include <cstdint>

namespace PiSubmarine::Max1726
{
	enum class RegOffset : uint8_t
	{
		Status = 0x00,
		VAlrtTh = 0x01,
		TAlrtTh = 0x02,
		SAlrtTh = 0x03,
		AtRate = 0x04,
		RepCap = 0x05,
		RepSOC = 0x06,
		Age = 0x07,
		Temp = 0x08,
		VCell = 0x09,
		Current = 0x0A,
		AvgCurrent = 0x0B,
		QResidual = 0x0C,
		MixSOC = 0x0D,
		AvSOC = 0x0E,
		MixCap = 0x0F,
		FullCapRep = 0x10,
		TTE = 0x11,
		QRTable00 = 0x12,
		FullSocThr = 0x13,
		RCell = 0x14,
		AvgTA = 0x16,
		Cycles = 0x17,
		DesignCap = 0x18,
		AvgVCell = 0x19,
		MaxMinTemp = 0x1A,
		MaxMinVolt = 0x1B,
		MaxMinCurr = 0x1C,
		Config = 0x1D,
		IChgTerm = 0x1E,
		TTF = 0x20,
		DevName = 0x21,
		QRTable10 = 0x22,
		FullCapNom = 0x23,
		AIN = 0x27,
		LearnCfg = 0x28,
		FilterCfg = 0x29,
		RelaxCfg = 0x2A,
		MiscCfg = 0x2B,
		TGain = 0x2C,
		TOff = 0x2D,
		CGain = 0x2E,
		COff = 0x2F,
		QRTable20 = 0x32,
		DieTemp = 0x34,
		FullCap = 0x35,
		RComp0 = 0x38,
		TempCo = 0x39,
		VEmpty = 0x3A,
		FStat = 0x3D,
		Timer = 0x3E,
		ShdnTimer = 0x3F,
		QRTable30 = 0x42,
		RGain = 0x43,
		dQAcc = 0x45,
		dPAcc = 0x46,
		ConvgCfg = 0x49,
		VFRemCap = 0x4A,
		QH = 0x4D,
		Status2 = 0xB0,
		Power = 0xB1,
		ID = 0xB2,
		AvgPower = 0xB3,
		IAlrtTh = 0xB4,
		TTFCfg = 0xB5,
		CVMixCap = 0xB6,
		CVHalfTime = 0xB7,
		CGTempCo = 0xB8,
		Curve = 0xB9,
		HibCfg = 0xBA,
		Config2 = 0xBB,
		VRipple = 0xBC,
		RippleCfg = 0xBD,
		TimerH = 0xBE,
		RSense = 0xD0,
		ScOcvLim = 0xD1,
		VGain = 0xD2,
		SOCHold = 0xD3,
		MaxPeakPower = 0xD4,
		SusPeakPower = 0xD5,
		PackResistance = 0xD6,
		SysResistance = 0xD7,
		MinSysVoltage = 0xD8,
		MPPCurrent = 0xD9,
		SPPCurrent = 0xDA,
		ModelCfg = 0xDB,
		AtQResidual = 0xDC,
		AtTTE = 0xDD,
		AtAvSOC = 0xDE,
		AtAvCap = 0xDF
	};

	enum class Status : uint16_t
	{
		PowerOnReset = (1 << 1),
		MinimumCurrentAlert = (1 << 2),
		BatteryPresent = (1 << 3),
		MaximumCurrentAlert = (1 << 6),
		StateOfChargeChanged = (1 << 7),
		MinimumVoltageAlert = (1 << 8),
		MinimumTemperatureAlert = (1 << 9),
		MinimumStateOfCharge = (1 << 10),
		MaximumVoltageAlert = (1 << 12),
		MaximumTemperatureAlert = (1 << 13),
		MaximumStateOfChargeAlert = (1 << 14),
		BatteryRemoved = (1 << 15)
	};

	constexpr static size_t MemorySize = 225;

	template<PiSubmarine::Api::Internal::I2C::DriverConcept I2CDriver>
	class Device
	{
	public:
		constexpr static uint8_t Address = 0x36;

		Device(I2CDriver& driver) : m_Driver(driver)
		{

		}

		/// <summary>
		/// Returns true if there is a pending read/write I2C transation.
		/// </summary>
		/// <returns>True if transaction not finished.</returns>
		bool IsTransactionInProgress()
		{
			return m_IsTransactionInProgress;
		}

		/// <summary>
		/// Returns true if previous transaction failed. Cleared to false upon new Read or Write.
		/// </summary>
		/// <returns>True if has error.</returns>
		bool HasError()
		{
			return m_HasError;
		}

		/// <summary>
		/// Reads all registers.
		/// </summary>
		/// <returns>True if transaction was successfully started.</returns>
		bool Read()
		{
			std::bitset<MemorySize> regs;
			regs.set();
			return Read(0, m_MemoryBuffer.data(), m_MemoryBuffer.size(), regs);
		}

		/// <summary>
		/// Reads specific register.
		/// </summary>
		/// <param name="reg">Register offset</param>
		/// <returns>True if transaction was successfully started.</returns>
		bool Read(RegOffset reg)
		{
			std::bitset<MemorySize> regs;
			regs.set(RegUtils::ToInt(reg));
			constexpr size_t regSize = 2;
			return Read(static_cast<uint8_t>(reg), m_MemoryBuffer.data() + static_cast<size_t>(reg), regSize, regs);
		}

		/// <summary>
		/// Writes specific register.
		/// </summary>
		/// <param name="reg">Register offset</param>
		/// <returns>True if transaction was successfully started.</returns>
		bool Write(RegOffset reg)
		{
			constexpr size_t regSize = 2;
			std::bitset<MemorySize> regs;
			regs.set(RegUtils::ToInt(reg));
			return Write(static_cast<uint8_t>(reg), m_MemoryBuffer.data() + static_cast<size_t>(reg), regSize, regs);
		}

		/// <summary>
		/// Writes all dirty registers in a sequence of transactions.
		/// </summary>
		/// <returns>True if transaction was successfully started. False if there was an error or no register was dirty.</returns>
		bool WriteDirty()
		{
			if (m_IsTransactionInProgress)
			{
				return false;
			}

			m_HasError = false;
			m_IsTransactionInProgress = true;

			return WriteDirtyInternal(RegOffset{ 0 });
		}

		bool HasDirtyRegisters()
		{
			return m_DirtyRegs.any();
		}

		Status GetStatus() const
		{
			return RegUtils::Read<Status, std::endian::big>(m_MemoryBuffer.data() + RegUtils::ToInt(RegOffset::Status), 0, 16);
		}

		void SetStatus(Status value)
		{
			RegUtils::Write<uint8_t, std::endian::big>(value, m_MemoryBuffer.data() + RegUtils::ToInt(RegOffset::Status), 0, 16);
			m_DirtyRegs[RegUtils::ToInt(RegOffset::Status)] = true;
		}

		
	private:
		I2CDriver& m_Driver;
		std::array<uint8_t, MemorySize> m_MemoryBuffer{ 0 };
		bool m_IsTransactionInProgress = false;
		bool m_HasError = false;
		std::bitset<MemorySize> m_DirtyRegs{ 0 };

		bool Read(uint8_t offset, uint8_t* data, size_t size, const std::bitset<MemorySize>& regs)
		{
			if (m_IsTransactionInProgress)
			{
				return false;
			}

			m_HasError = !m_Driver.Write(Address, &offset, 1);
			if (m_HasError)
			{
				return false;
			}

			bool transactionStarted = m_Driver.ReadAsync(Address, data, size, [this, regs](uint8_t cbAddress, bool cbOk) {ReadCallback(cbAddress, regs, cbOk); });
			if (transactionStarted)
			{
				m_IsTransactionInProgress = true;
			}

			return transactionStarted;
		}

		bool Write(uint8_t offset, uint8_t* data, size_t size, const std::bitset<MemorySize>& regs)
		{
			if (m_IsTransactionInProgress)
			{
				return false;
			}

			m_HasError = false;

			std::vector<uint8_t> buffer;
			buffer.resize(size + 1);
			buffer[0] = offset;
			memcpy(buffer.data() + 1, data, size);

			bool transactionStarted = m_Driver.WriteAsync(Address, buffer.data(), buffer.size(), [this, regs](uint8_t cbAddress, bool cbOk) {WriteCallback(cbAddress, regs, cbOk); });
			if (transactionStarted)
			{
				m_IsTransactionInProgress = true;
			}

			return transactionStarted;
		}

		void ReadCallback(uint8_t deviceAddress, std::bitset<MemorySize> regs, bool ok)
		{
			m_HasError = !ok;
			m_IsTransactionInProgress = false;

			if (ok)
			{
				m_DirtyRegs &= ~regs;
			}
		}

		void WriteCallback(uint8_t deviceAddress, std::bitset<MemorySize> regs, bool ok)
		{
			m_HasError = !ok;
			m_IsTransactionInProgress = false;

			if (ok)
			{
				m_DirtyRegs &= ~regs;
			}
		}

		bool WriteDirtyInternal(RegOffset regNext)
		{
			for (size_t i = RegUtils::ToInt(regNext); i < m_DirtyRegs.size(); i++)
			{
				if (!m_DirtyRegs[i])
				{
					continue;
				}
				RegOffset reg = static_cast<RegOffset>(i);

				uint8_t regSize = GetRegisterSize(reg);
				std::vector<uint8_t> buffer;
				buffer.resize(regSize + 1);
				buffer[0] = i;
				memcpy(buffer.data() + 1, m_MemoryBuffer.data() + i, regSize);
				return m_Driver.WriteAsync(Address, buffer.data(), buffer.size(), [this, reg](uint8_t cbAddress, bool cbOk) {WriteDirtyCallback(cbAddress, reg, cbOk); });
			}
			return false;
		}

		void WriteDirtyCallback(uint8_t deviceAddress, RegOffset reg, bool ok)
		{
			if (!ok)
			{
				m_HasError = true;
				m_IsTransactionInProgress = false;
				return;
			}

			m_HasError = false;
			m_DirtyRegs[RegUtils::ToInt(reg)] = false;
			if (m_DirtyRegs == 0)
			{
				m_IsTransactionInProgress = false;
				return;
			}

			if (!WriteDirtyInternal(static_cast<RegOffset>(RegUtils::ToInt(reg) + 1)))
			{
				m_HasError = true;
				m_IsTransactionInProgress = false;
				return;
			}
		}
	};
};