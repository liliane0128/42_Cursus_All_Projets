import { mediaUrl } from "../../api";

const SIZE: Record<"sm" | "md" | "lg", string> = {
  sm: "w-7 h-7 text-[11px]",
  md: "w-8 h-8 text-[11px]",
  lg: "w-16 h-16 text-2xl",
};

const VARIANT: Record<"indigo" | "dark", string> = {
  indigo: "bg-indigo-50 text-indigo-600 font-semibold",
  dark:   "bg-gray-700 text-white font-bold",
};

type Props = {
  src?: string | null;
  name: string;
  size?: "sm" | "md" | "lg";
  variant?: "indigo" | "dark";
  fallback?: string;
  onClick?: () => void;
  className?: string;
};

export default function Avatar({ src, name, size = "md", variant = "indigo", fallback, onClick, className = "" }: Props) {
  const url = mediaUrl(src);
  const base = `${SIZE[size]} ${VARIANT[variant]} rounded-full overflow-hidden flex items-center justify-center flex-shrink-0${onClick ? " cursor-pointer" : ""}${className ? ` ${className}` : ""}`;
  return (
    <div className={base} onClick={onClick}>
      {url
        ? <img src={url} alt={name} className="w-full h-full object-cover" />
        : <span>{fallback ?? name[0]?.toUpperCase() ?? "?"}</span>
      }
    </div>
  );
}
